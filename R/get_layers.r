go_get_ip_layer <- function(pcap, first_packet) {

  ip <- get_ip_layer(pcap)

  ip$flag_bits <- sapply(ip$flags, function(x) paste0(as.integer(intToBits(x))[1:3], collapse=""))
  ip <- merge(ip, protocol_numbers[, c("protocol", "protocol_name")], by.x="protocol", by.y="protocol")

  ip <- arrange(ip, tv_sec, tv_usec)

  ip$time <- (ip$tv_sec + (ip$tv_usec/1000000)) - (first_packet$tv_sec + (first_packet$tv_usec/1000000))

  ip[,c("tv_sec", "tv_usec", "time", "src", "dst", "protocol", "protocol_name",
        "size", "header_len", "total_len", "ttl",
        "flags", "flag_bits", "dscp", "frag_ofs")]

}

