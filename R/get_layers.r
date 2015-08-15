go_get_ip_layer <- function(pcap, first_packet) {

  ip <- get_ip_layer(pcap)

  ip$flag_bits <- sapply(ip$flags,
                         function(x) paste0(as.integer(intToBits(x))[1:3],
                                            collapse=""))
  ip <- merge(ip, protocol_numbers[, c("protocol", "protocol_name")],
              by.x="protocol", by.y="protocol")

  ip <- arrange(ip, num)

  ip[,c("num", "tv_sec", "tv_usec", "src", "dst", "protocol",
        "protocol_name", "size", "header_len", "total_len", "ttl", "flags",
        "flag_bits", "dscp", "frag_ofs")]

}

