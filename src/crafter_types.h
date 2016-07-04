#include <Rcpp.h>
#include <crafter.h>

typedef Crafter::Packet* MyPacket;

void pcap_finalizer(std::vector<Crafter::Packet*>* pcap);
void pcap_packet_finalizer(MyPacket *pcap_packet);

typedef Rcpp::XPtr<std::vector<Crafter::Packet*>, Rcpp::PreserveStorage, pcap_finalizer> pcapptr;
typedef Rcpp::XPtr<MyPacket, Rcpp::PreserveStorage, pcap_packet_finalizer> pcap_packet_ptr;
