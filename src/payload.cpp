#include <crafter.h>
#include <iostream>
#include <Rcpp.h>

#include "crafter_types.h"

using namespace Rcpp;
using namespace std;
using namespace Crafter;


// [[Rcpp::export]]
CharacterVector get_payload_for(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap,
                                int packet_num) {

  std::vector<Packet*> pck_cont = *pcap;
  RawLayer* raw = pck_cont[packet_num-1]->GetLayer<RawLayer>();
  if (raw) {
    return(raw->GetStringPayload());
  } else {
    return(NA_STRING);
  }

}

