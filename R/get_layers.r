go_get_ip_layer <- function(pcap, first_packet) {

  ip <- get_ip_layer(pcap)

  ip$flag_bits <- sapply(ip$flags,
                         function(x) paste0(as.integer(intToBits(x))[1:3],
                                            collapse=""))
  ip <- merge(ip, protocol_numbers[, c("protocol", "protocol_name")],
              by.x="protocol", by.y="protocol")

  ip <- arrange(ip, num)

  ip[,c("num", "tv_sec", "tv_usec", "src", "dst",
        "protocol_name", "size", "header_len", "total_len", "ttl", "flags",
        "flag_bits", "dscp", "frag_ofs")]

}

go_get_tcp_layer <- function(pcap, first_packet) {

  tcp <- get_tcp_layer(pcap)

  tcp <- merge(tcp, protocol_numbers[, c("protocol", "protocol_name")],
              by.x="protocol", by.y="protocol")

  tcp <- arrange(tcp, num)

  tcp[,c("num", "tv_sec", "tv_usec", "src", "dst",
        "protocol_name", "srcport", "dstport", "seqnum", "acknum",
        "fin", "syn", "rst", "psh", "ack", "urg", "ece", "cwr")]

}

go_get_icmp_layer <- function(pcap, first_packet) {

  icmp <- get_icmp_layer(pcap)

  icmp <- merge(icmp, protocol_numbers[, c("protocol", "protocol_name")],
              by.x="protocol", by.y="protocol")

  icmp <- merge(icmp, icmp_type_numbers,
              by.x="icmptype", by.y="icmptype")

  icmp <- arrange(icmp, num)

  icmp[,c("num", "tv_sec", "tv_usec", "src", "dst",
        "protocol_name", "identifier", "seqnum", "icmptype", "icmpname",
        "code", "chksum")]

}
