#' Read in a packet capture file
#'
#' Reads in a packet capture file and returns an hyrid S3 object with internal
#' methods to call to operate on the capture.
#'
#' @param capture_file path to a PCAP
#' @param filter filter
#' @references \href{http://www.tcpdump.org/manpages/pcap-filter.7.html}{pcap filter reference}
#' @export
read_pcap <- function(capture_file, filter="") {

  # Private fields
  private <- environment();

  # Public methods
  this <- local({

    filename <- path.expand(capture_file)
    filter_applied <- filter;

    private$pcap <- load_pcap(filename, filter)

    #' @describeIn packet_count total # of packets in the capture
    packet_count <- num_packets(private$pcap)

    #' @describeIn packet_info retrieve high level packet information
    packet_info <- function() {
      dat <- get_packet_info(private$pcap)
      dat$protocols <- sapply(str_split(dat$protocols, ","), function(x) paste0(sort(unique(x)), collapse=","))
      dat <- arrange(dat, tv_sec, tv_usec)
      dat$time <- (dat$tv_sec + (dat$tv_usec/1000000)) - (dat[1,]$tv_sec + (dat[1,]$tv_usec/1000000))
      dat[,c("tv_sec", "tv_usec", "time", "layer_count", "protocols", "packet_size")]
    }

    first_packet <- packet_info()[1,]

    #' @describeIn get_layer retrieve a specific layer type
    get_layer <- function(layer_name=c("Ethernet", "IP", "TCP", "TCPOptionMaxSegSize", "TCPOptionPad", "TCPOptionSACKPermitted")) {
      switch(layer_name,
#             Ethernet=get_ethernet_layer(private$pcap),
#             TCP=get_tcp_layer(private$pcap),
             IP=go_get_ip_layer(private$pcap, first_packet),
             stop(sprintf("'%s' decoding not implemented yet", layer_name)))
    }

    #' @describeIn summary get summary of the packet capture
    summary <- function() {

      pkts <- packet_info()
      first <- pkts[1,]
      last <- pkts[nrow(pkts),]

      pktfil <- file.info(filename)

      ts <- (last$tv_sec - first$tv_sec) + ((last$tv_usec - first$tv_usec)/1000000)

      cat(red("File\n"))
      cat(sprintf(" Capture file  : %s\n", filename))
      cat(sprintf(" Filter applied: %s\n", ifelse(filter=="", "[none]", filter)))
      cat(sprintf(" Length        : %d bytes\n", pktfil$size))
      cat("\n")
      cat(red("Time\n"))
      cat(sprintf(" First packet: %s\n",  as.POSIXct(first$tv_sec, origin="1970-01-01 00:00:00", tz="UTC")))
      cat(sprintf(" Last packet : %s\n",  as.POSIXct(last$tv_sec, origin="1970-01-01 00:00:00", tz="UTC")))
      cat("\n")
      cat(red("Statistics\n"))
      cat(sprintf(" Packets      : %s\n", packet_count))
      cat(sprintf(" Time span (s): %0.3f\n", ts))
      cat(sprintf(" Average pps  : %0.1f\n", packet_count / ts))

    }

    lockEnvironment(environment(), TRUE)
    structure(environment(), class=c("crafter", "environment"))

  })

}

#' Print summary info about a packet capture
#'
#' @export
summary.crafter <- function(pcap) {
  pcap$summary()
}
