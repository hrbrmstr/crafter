#include <crafter.h>
#include <iostream>
#include <Rcpp.h>

#include "crafter_types.h"

using namespace Rcpp;
using namespace std;
using namespace Crafter;


// get the tcp layer from a pcap as a data frame but we
// can't due to Rcpp limitation in DataFrame::create so we
// have to make it a List and do as.data.frame in R
//
// @param pcap pcap
// [[Rcpp::export]]
List get_tcp_layer(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap) {

  std::vector<Packet*> pck_cont = *pcap;

  std::vector<int> num; num.reserve(pck_cont.size());
  std::vector<int> tssec; tssec.reserve(pck_cont.size());
  std::vector<int> tsusec; tsusec.reserve(pck_cont.size());
  std::vector<std::string> src; src.reserve(pck_cont.size());
  std::vector<std::string> dst; dst.reserve(pck_cont.size());
  std::vector<int> protocol; protocol.reserve(pck_cont.size());
  std::vector<int> srcport; srcport.reserve(pck_cont.size());
  std::vector<int> dstport; dstport.reserve(pck_cont.size());
  std::vector<unsigned int> seqnum; seqnum.reserve(pck_cont.size());
  std::vector<unsigned int> acknum; acknum.reserve(pck_cont.size());
  std::vector<int> headersize; headersize.reserve(pck_cont.size());
  std::vector<int> payloadsize; payloadsize.reserve(pck_cont.size());
  std::vector<bool> fin; fin.reserve(pck_cont.size());
  std::vector<bool> syn; syn.reserve(pck_cont.size());
  std::vector<bool> rst; rst.reserve(pck_cont.size());
  std::vector<bool> psh; psh.reserve(pck_cont.size());
  std::vector<bool> ack; ack.reserve(pck_cont.size());
  std::vector<bool> urg; urg.reserve(pck_cont.size());
  std::vector<bool> ece; ece.reserve(pck_cont.size());
  std::vector<bool> cwr; cwr.reserve(pck_cont.size());
  std::vector<std::string> payload; payload.reserve(pck_cont.size());
  std::vector<unsigned int> chksum; chksum.reserve(pck_cont.size());
  std::vector<unsigned int> optsize; optsize.reserve(pck_cont.size());
  std::vector<unsigned int> windowsize; windowsize.reserve(pck_cont.size());

  std::vector<Packet*>::iterator it_pck;
  timeval ts;

  int i = 0;
  for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {

    TCP* tcp_layer = (*it_pck)->GetLayer<TCP>();
    IP* ip_layer = (*it_pck)->GetLayer<IP>();

    if (tcp_layer) {

      RawLayer* raw_layer = (*it_pck)->GetLayer<RawLayer>();

      ts = (*it_pck)->GetTimestamp();

      num.push_back(i+1);
      tssec.push_back(ts.tv_sec);
      tsusec.push_back(ts.tv_usec);
      src.push_back(ip_layer->GetSourceIP());
      dst.push_back(ip_layer->GetDestinationIP());
      protocol.push_back((int)ip_layer->GetProtocol());
      srcport.push_back((int)tcp_layer->GetSrcPort());
      dstport.push_back((int)tcp_layer->GetDstPort());
      seqnum.push_back((unsigned int)tcp_layer->GetSeqNumber());
      acknum.push_back((unsigned int)tcp_layer->GetAckNumber());
      headersize.push_back((int)tcp_layer->GetHeaderSize());
      if (raw_layer) {
        payloadsize.push_back((int)raw_layer->GetPayloadSize());
        payload.push_back(raw_layer->GetStringPayload());
      } else {
        payloadsize.push_back((int)tcp_layer->GetPayloadSize());
        payload.push_back("");
      }
      fin.push_back((int)tcp_layer->GetFIN());
      syn.push_back((int)tcp_layer->GetSYN());
      rst.push_back((int)tcp_layer->GetRST());
      psh.push_back((int)tcp_layer->GetPSH());
      ack.push_back((int)tcp_layer->GetACK());
      urg.push_back((int)tcp_layer->GetURG());
      ece.push_back((int)tcp_layer->GetECE());
      cwr.push_back((int)tcp_layer->GetCWR());
      windowsize.push_back((unsigned int)tcp_layer->GetWindowsSize());
      chksum.push_back((unsigned int)tcp_layer->GetCheckSum());
      optsize.push_back((unsigned int)tcp_layer->ComputeOptionSize());

    }

    i++;

  }

  // can only make a List or DataFramw with up to 20 elements so we
  // have to split them into 2 and return a list and then cbind them
  // together in R

  DataFrame ret1 = DataFrame::create(_["num"] = num,
                      _["tv_sec"] = tssec,
                      _["tv_usec"] = tsusec,
                      _["src"] = src,
                      _["dst"] = dst,
                      _["protocol"] = protocol,
                      _["srcport"] = srcport,
                      _["dstport"] = dstport,
                      _["seqnum"] = seqnum,
                      _["acknum"] = acknum,
                      _["headersize"] = headersize,
                      _["payloadsize"] = payloadsize,
                      _["fin"] = fin,
                      _["syn"] = syn,
                      _["rst"] = rst,
                      _["psh"] = psh,
                      _["ack"] = ack,
                      _["urg"] = urg,
                      _["ece"] = ece,
                      _["stringsAsFactors"] = false);

  DataFrame ret2 = DataFrame::create(_["payload"] = payload,
                           _["cwr"] = cwr,
                           _["windowsize"] = windowsize,
                           _["chksum"] = chksum,
                           _["optsize"] = optsize,
                           _["stringsAsFactors"] = false);

  return(List::create(ret1, ret2));

}

