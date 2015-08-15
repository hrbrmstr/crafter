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

    first_packet <- function() { private$first }
    last_packet <- function() { private$last }

    #' @describeIn packet_info retrieve high level packet information
    packet_info <- function() {
      dat <- get_packet_info(private$pcap)
      dat$protocols <- sapply(str_split(dat$protocols, ","),
                              function(x) paste0(sort(unique(x)), collapse=","))
      dat <- arrange(dat, tv_sec, tv_usec)
      private$first_packet <- dat[1,]
      private$last_packet <- dat[nrow(dat),]
      dat <- arrange(dat, num)
      dat[,c("num", "tv_sec", "tv_usec", "layer_count",
             "protocols", "packet_size")]
    }

    #' @describeIn get_layer retrieve a specific layer type
    get_layer <- function(layer_name=c("Ethernet", "IP", "TCP", "ICMP",
                                       "TCPOptionMaxSegSize", "TCPOptionPad",
                                       "TCPOptionSACKPermitted")) {
      switch(layer_name,
             icmp=,
             ICMP=go_get_icmp_layer(private$pcap, first_packet()),
             tcp=,
             TCP=go_get_tcp_layer(private$pcap, first_packet()),
             ip=,
             IP=go_get_ip_layer(private$pcap, first_packet()),
             stop(sprintf("'%s' decoding not implemented yet", layer_name)))
    }

    #' @describeIn summary get summary of the packet capture
    summary <- function() {

      pkts <- packet_info()

      pktfil <- file.info(filename)

      ts <- (private$last_packet$tv_sec - private$first_packet$tv_sec) +
        ((private$last_packet$tv_usec - private$first_packet$tv_usec)/1000000)

      cat(red("File\n"))
      cat(sprintf(" Capture file  : %s\n", filename))
      cat(sprintf(" Filter applied: %s\n", ifelse(filter=="", "[none]", filter)))
      cat(sprintf(" Length        : %d bytes\n", pktfil$size))
      cat("\n")
      cat(red("Time\n"))
      cat(sprintf(" First packet: %s\n",  as.POSIXct(private$first_packet$tv_sec, origin="1970-01-01 00:00:00", tz="UTC")))
      cat(sprintf(" Last packet : %s\n",  as.POSIXct(private$last_packet$tv_sec, origin="1970-01-01 00:00:00", tz="UTC")))
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
