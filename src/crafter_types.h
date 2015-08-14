#include <crafter.h>
#include <Rcpp.h>
void pcap_finalizer( std::vector<Crafter::Packet*>* pcap );
typedef Rcpp::XPtr<std::vector<Crafter::Packet*>, Rcpp::PreserveStorage, pcap_finalizer> pcapptr;
