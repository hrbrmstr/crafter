#' Read in a packet capture file
#'
#' Reads in a packet capture file and returns an hyrid S3 \code{crafter} object
#' with internal methods and accessible variables.
#'
#' Callable internal methods & accesible variables  of the \code{crafter} object:
#'
#' \describe{
#'   \item{\code{packet_count}}{Total packets in capture}
#'   \item{\code{get_ips(src_or_dst)}}{Retrieve a data frame of IP addresses.
#'         "\code{src_or_dst}" can be one of "\code{src}", "\code{dst}" or "\code{all}" }
#'   \item{\code{packet_info()}}{Retrieve a data frame of high-level packet information}
#'   \item{\code{get_payload(num)}}{Retrieve the payload of a specific packet number}
#'   \item{\code{get_layer(layer_name)}}{Get all the packets from a layer,
#'        "\code{layer_name}" can be one of \code{TCP, IP, ICMP}}
#'   \item{\code{summary()}}{Prints summary information about the packet capture}
#' }
#'
#' @param capture_file path to a PCAP
#' @param filter filter
#' @return A \code{crafter} hybrid S3 object
#' @references \href{http://www.tcpdump.org/manpages/pcap-filter.7.html}{pcap filter reference}
#' @export
#' @examples
#' ncap <- read_pcap(system.file("pcaps/Ncapture.pcap", package="crafter"))
#'
#' # get overall capture summary
#' summary(ncap)
#'
#' # same thing
#' ncap$summary()
#'
#' # high level packet info
#' ncap_info <- ncap$packet_info()
#' head(ncap_info)
#'
#' # retrieve specific layers
#' ncap_ip <- ncap$get_layer("IP")
#' head(ncap_ip)
#'
#' ncap_tcp <- ncap$get_layer("TCP")
#' head(ncap_tcp)
#'
#' # get the IPs from the capture
#' ncap$get_ips("all")
#'
#' # get packet payload
#' ncap$get_payload(7119)
read_pcap <- function(capture_file, filter = "") {
  # Private fields
  private <- environment()

  # Public methods
  this <- local({
    filename <- path.expand(capture_file)

    filter_applied <- filter

    private$pcap <- load_pcap(filename, filter)

    #' @describeIn packet_count total # of packets in the capture
    packet_count <- num_packets(private$pcap)

    first_packet <- function() {
      private$first
    }
    last_packet <- function() {
      private$last
    }

    get_ips <- function(src_or_dst="src") {
      switch(tolower(src_or_dst),
        source,
        src=return(count(data_frame(ip=crafter_get_ips(private$pcap, src_or_dst)), ip)),
        dest,
        destination,
        dst=return(count(data_frame(ip=crafter_get_ips(private$pcap, src_or_dst)), ip)),
        both,
        all=return(count(bind_rows(data_frame(ip=crafter_get_ips(private$pcap, "src")),
                                   data_frame(ip=crafter_get_ips(private$pcap, "dst"))), ip)),
        stop("'src_or_dst' must be one of 'src', 'dst' or 'dst'", call.=FALSE)
      )

    }

    #' @describeIn packet_info retrieve high level packet information
    packet_info <- function() {
      dat <- get_packet_info(private$pcap)
      dat$protocols <- sapply(str_split(dat$protocols, ","),
                              function(x)
                                paste0(sort(unique(x)), collapse = ","))
      dat <- arrange(dat, tv_sec, tv_usec)
      private$first_packet <- dat[1, ]
      private$last_packet <- dat[nrow(dat), ]
      dat <- arrange(dat, num)
      dat[, c("num",
              "tv_sec",
              "tv_usec",
              "layer_count",
              "protocols",
              "packet_size")]
    }

    get_payload <- function(num) {
      get_payload_for(private$pcap, num)
    }

    #' @describeIn get_layer retrieve a specific layer type
    get_layer <-
      function(layer_name = c(
        "Ethernet",
        "IP",
        "TCP",
        "ICMP",
        "TCPOptionMaxSegSize",
        "TCPOptionPad",
        "TCPOptionSACKPermitted"
      )) {
        switch(tolower(layer_name),
          icmp = go_get_icmp_layer(private$pcap, first_packet()),
          tcp = go_get_tcp_layer(private$pcap, first_packet()),
          ip = go_get_ip_layer(private$pcap, first_packet()),
          stop(sprintf(
            "'%s' decoding not implemented yet", layer_name
          ))
        )
      }

    #' @describeIn summary get summary of the packet capture
    summary <- function() {
      pkts <- packet_info()

      pktfil <- file.info(filename)

      ts <-
        (private$last_packet$tv_sec - private$first_packet$tv_sec) +
        ((
          private$last_packet$tv_usec - private$first_packet$tv_usec
        ) / 1000000)

      cat(red("File\n"))
      cat(sprintf(" Capture file  : %s\n", filename))
      cat(sprintf(
        " Filter applied: %s\n",
        ifelse(filter == "", "[none]", filter)
      ))
      cat(sprintf(" Length        : %d bytes\n", pktfil$size))
      cat("\n")
      cat(red("Time\n"))
      cat(sprintf(
        " First packet: %s\n",
        as.POSIXct(
          private$first_packet$tv_sec,
          origin = "1970-01-01 00:00:00",
          tz = "UTC"
        )
      ))
      cat(sprintf(
        " Last packet : %s\n",
        as.POSIXct(
          private$last_packet$tv_sec,
          origin = "1970-01-01 00:00:00",
          tz = "UTC"
        )
      ))
      cat("\n")
      cat(red("Statistics\n"))
      cat(sprintf(" Packets      : %s\n", packet_count))
      cat(sprintf(" Time span (s): %0.3f\n", ts))
      cat(sprintf(" Average pps  : %0.1f\n", packet_count / ts))

    }

    lockEnvironment(environment(), TRUE)
    structure(environment(), class = c("crafter", "environment"))

  })

}

#' Print summary info about a packet capture
#'
#' @export
summary.crafter <- function(pcap) {
  pcap$summary()
}

#' Print method
#'
#' @export
print.crafter <- function(pcap) {
  print(pcap$packet_info())
}
