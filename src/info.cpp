#include <crafter.h>
#include <iostream>
#include <Rcpp.h>

#include "crafter_types.h"

using namespace Rcpp;
using namespace std;
using namespace Crafter;


// Get general packet info (high level) and return a data frame
//
// [[Rcpp::export]]
DataFrame get_packet_info(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap) {

  std::vector<Packet*> pck_cont = *pcap;

  std::vector<int> num(pck_cont.size());
  std::vector<int> tssec(pck_cont.size());
  std::vector<int> tsusec(pck_cont.size());
  std::vector<int> layer_count(pck_cont.size());
  std::vector<std::string> protos(pck_cont.size());
  std::vector<int> packet_size(pck_cont.size());

  std::vector<Packet*>::iterator it_pck;
  timeval ts ;

  int i=0;
  for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {
    num[i] = i+1;
    layer_count[i] = (*it_pck)->GetLayerCount();
    packet_size[i] = (*it_pck)->GetSize();
    ts = (*it_pck)->GetTimestamp();
    tssec[i] = ts.tv_sec;
    tsusec[i] = ts.tv_usec;
    int j = 0;
    protos[i] = "" + (*it_pck)->GetLayer<Layer>(j)->GetName();
    for (j=1; j<layer_count[i]; j++) {
      protos[i] = protos[i] + "," + (*it_pck)->GetLayer<Layer>(j)->GetName();
    }
    i++;
  }

  DataFrame ret1 = DataFrame::create(_["num"] = num,
                           _["tv_sec"] = tssec,
                           _["tv_usec"] = tsusec,
                           _["layer_count"] = layer_count,
                           _["protocols"] = protos,
                           _["packet_size"] = packet_size,
                           _["stringsAsFactors"] = false);

  ret1.attr("class") = CharacterVector::create("tbl_df", "tbl", "data.frame");
  return(ret1);

}
