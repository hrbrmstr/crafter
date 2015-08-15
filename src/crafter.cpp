#include <crafter.h>
#include <iostream>
#include <Rcpp.h>

using namespace Rcpp;
using namespace std;
using namespace Crafter;

/* a linked list keeping track of pcaps */
struct node {
  std::vector<Packet*> pcap;
  int link_type ;
  node *next;
};

node pcaplist = *(new node);
node *lstail = &pcaplist;

void pcap_finalizer( std::vector<Packet*>* pcap ) { }

typedef XPtr<std::vector<Packet*>, PreserveStorage, pcap_finalizer> pcapptr;

template<class Seq>
void ReadPcap(Seq* pck_container, int *link_type, const std::string& filename, const std::string& filter = "") {
  /* Handle for the opened pcap session */
  pcap_t *handle;

  OpenOffPcap(link_type,handle,filename,filter);

  /* Prepare the data */
  ThreadReadData<Seq> rd;
  rd.link_type = *link_type;
  rd.pck_container = pck_container;

  u_char* arg = reinterpret_cast<u_char*>(&rd);

  LoopPcap(handle,-1,process_thread<Seq>,arg);

  ClosePcap(handle);

}

// read in a pcap file
//
// @param capture file path to capture file
// [[Rcpp::export]]
pcapptr load_pcap(std::string capture_file, std::string filter) {

  ReadPcap(&(lstail->pcap), &(lstail->link_type), capture_file, filter);
  pcapptr ptr(&(lstail->pcap));
  lstail-> next = new node;
  lstail = lstail->next;
  return(ptr);

}

// get number of packets in capture
//
// @param pcap pcap
// [[Rcpp::export]]
int num_packets(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap) {

  if (!pcap)
    throw std::runtime_error("Something is terribly wrong here.");

  return(pcap->size());

}

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

  return DataFrame::create(_["num"] = num,
                           _["tv_sec"] = tssec,
                           _["tv_usec"] = tsusec,
                           _["layer_count"] = layer_count,
                           _["protocols"] = protos,
                           _["packet_size"] = packet_size,
                           _["stringsAsFactors"] = false);


}

// get the ip layer from a pcap as a data frame
//
// @param pcap pcap
// [[Rcpp::export]]
DataFrame get_ip_layer(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap) {

  std::vector<Packet*> pck_cont = *pcap;

  std::vector<int> num; num.reserve(pck_cont.size());
  std::vector<int> tssec; tssec.reserve(pck_cont.size());
  std::vector<int> tsusec; tsusec.reserve(pck_cont.size());
  std::vector<int> sz; sz.reserve(pck_cont.size());
  std::vector<std::string> src; src.reserve(pck_cont.size());
  std::vector<std::string> dst; dst.reserve(pck_cont.size());
  std::vector<int> protocol; protocol.reserve(pck_cont.size());
  std::vector<int> ttl; ttl.reserve(pck_cont.size());
  std::vector<int> dscp; dscp.reserve(pck_cont.size());
  std::vector<int> flags; flags.reserve(pck_cont.size());
  std::vector<int> version; version.reserve(pck_cont.size());
  std::vector<int> fragofs; fragofs.reserve(pck_cont.size());
  std::vector<int> headerlen; headerlen.reserve(pck_cont.size());
  std::vector<int> totallen; totallen.reserve(pck_cont.size());

  std::vector<Packet*>::iterator it_pck;
  timeval ts;

  int i = 0;
  for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {

    IP* ip_layer = (*it_pck)->GetLayer<IP>();

    if (ip_layer) {

      ts = (*it_pck)->GetTimestamp();

      num.push_back(i+1);
      tssec.push_back(ts.tv_sec);
      tsusec.push_back(ts.tv_usec);
      sz.push_back((*it_pck)->GetSize());
      src.push_back(ip_layer->GetSourceIP());
      dst.push_back(ip_layer->GetDestinationIP());
      protocol.push_back((int)ip_layer->GetProtocol());
      ttl.push_back((int)ip_layer->GetTTL());
      dscp.push_back((int)ip_layer->GetDiffServicesCP());
      flags.push_back(ip_layer->GetFlags());
      version.push_back(ip_layer->GetVersion());
      fragofs.push_back(ip_layer->GetFragmentOffset());
      headerlen.push_back(ip_layer->GetHeaderLength());
      totallen.push_back(ip_layer->GetTotalLength());

    }

    i++;

  }

  return DataFrame::create(_["num"] = num,
                           _["tv_sec"] = tssec,
                           _["tv_usec"] = tsusec,
                           _["src"] = src,
                           _["dst"] = dst,
                           _["protocol"] = protocol,
                           _["size"] = sz,
                           _["header_len"] = headerlen,
                           _["total_len"] = totallen,
                           _["ttl"] = ttl,
                           _["flags"] = flags,
                           _["dscp"] = dscp,
                           _["frag_ofs"] = fragofs,
                           _["stringsAsFactors"] = false);

}


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

    }

    i++;

  }

  List ret1 = List::create(_["num"] = num,
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
                      _["cwr"] = cwr);

  List ret2 = List::create(_["payload"] = payload);

  return(List::create(ret1, ret2));

}


// get the icmp layer from a pcap as a data frame
//
// @param pcap pcap
// [[Rcpp::export]]
DataFrame get_icmp_layer(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap) {

  std::vector<Packet*> pck_cont = *pcap;

  std::vector<int> num; num.reserve(pck_cont.size());
  std::vector<int> tssec; tssec.reserve(pck_cont.size());
  std::vector<int> tsusec; tsusec.reserve(pck_cont.size());
  std::vector<std::string> src; src.reserve(pck_cont.size());
  std::vector<std::string> dst; dst.reserve(pck_cont.size());
  std::vector<int> protocol; protocol.reserve(pck_cont.size());
  std::vector<unsigned int> identifier; identifier.reserve(pck_cont.size());
  std::vector<unsigned int> seqnum; seqnum.reserve(pck_cont.size());
  std::vector<int> icmptype; icmptype.reserve(pck_cont.size());
  std::vector<int> code; code.reserve(pck_cont.size());
  std::vector<unsigned int> chksum; chksum.reserve(pck_cont.size());


  std::vector<Packet*>::iterator it_pck;
  timeval ts;

  int i = 0;
  for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {

    ICMP* icmp_layer = (*it_pck)->GetLayer<ICMP>();
    IP* ip_layer = (*it_pck)->GetLayer<IP>();

    if (icmp_layer) {

      ts = (*it_pck)->GetTimestamp();

      num.push_back(i+1);
      tssec.push_back(ts.tv_sec);
      tsusec.push_back(ts.tv_usec);
      src.push_back(ip_layer->GetSourceIP());
      dst.push_back(ip_layer->GetDestinationIP());
      protocol.push_back((int)ip_layer->GetProtocol());
      identifier.push_back((unsigned int)icmp_layer->GetIdentifier());
      seqnum.push_back((unsigned int)icmp_layer->GetSequenceNumber());
      icmptype.push_back((int)icmp_layer->GetType());
      code.push_back((int)icmp_layer->GetCode());
      chksum.push_back((unsigned int)icmp_layer->GetCheckSum());

    }

    i++;

  }

  return DataFrame::create(_["num"] = num,
                           _["tv_sec"] = tssec,
                           _["tv_usec"] = tsusec,
                           _["src"] = src,
                           _["dst"] = dst,
                           _["protocol"] = protocol,
                           _["identifier"] = identifier,
                           _["seqnum"] = seqnum,
                           _["icmptype"] = icmptype,
                           _["code"] = code,
                           _["chksum"] = chksum,
                           _["stringsAsFactors"] = false);

}
