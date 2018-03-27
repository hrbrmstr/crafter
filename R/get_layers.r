# helper function to retrieve the IP layer, add some extra meta info
# and return sorted by packet
go_get_ip_layer <- function(pcap, first_packet) {

  ip <- get_ip_layer(pcap)

  ip$flag_bits <- sapply(ip$flags,
                         function(x) paste0(as.integer(intToBits(x))[1:3],
                                            collapse=""))
  ip <- merge(ip, protocol_numbers[, c("protocol", "protocol_name")],
              by.x="protocol", by.y="protocol")

  ip <- dplyr::arrange(ip, num)

  out <- ip[,c("num", "tv_sec", "tv_usec", "src", "dst",
        "protocol_name", "size", "header_len", "total_len", "ttl", "flags",
        "flag_bits", "dscp", "frag_ofs")]

  class(out) <- c("tbl_df", "tbl", "data.frame")

  out

}

# helper function to retrieve the TCP layer, add some extra meta info
# and return sorted by packet #
go_get_tcp_layer <- function(pcap, first_packet) {

  # max size for Rcpp::List and Rcpp:DataFrame is 20 members so
  # we have to cbind here
  tcp <- do.call(cbind.data.frame, get_tcp_layer(pcap))

  tcp <- merge(tcp, protocol_numbers[, c("protocol", "protocol_name")],
              by.x="protocol", by.y="protocol")

  tcp <- dplyr::arrange(tcp, num)

  out <- tcp[,c("num", "tv_sec", "tv_usec", "src", "dst",
        "protocol_name", "srcport", "dstport", "seqnum", "acknum",
        "headersize", "payloadsize",
        "fin", "syn", "rst", "psh", "ack", "urg", "ece", "cwr",
        "windowsize", "chksum", "optsize", "payload")]

  class(out) <- c("tbl_df", "tbl", "data.frame")

  out


}

# helper function to retrieve the ICMP layer, add some extra meta info
# and return sorted by packet #
go_get_icmp_layer <- function(pcap, first_packet) {

  icmp <- get_icmp_layer(pcap)

  icmp <- merge(icmp, protocol_numbers[, c("protocol", "protocol_name")],
              by.x="protocol", by.y="protocol")

  icmp <- merge(icmp, icmp_type_numbers,
              by.x="icmptype", by.y="icmptype")

  icmp <- dplyr::arrange(icmp, num)

  out <- icmp[,c("num", "tv_sec", "tv_usec", "src", "dst",
        "protocol_name", "identifier", "seqnum", "icmptype", "icmpname",
        "code", "chksum")]

  class(out) <- c("tbl_df", "tbl", "data.frame")

  out

}
