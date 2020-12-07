#ifndef CENTRALITYOUTPUTMANAGER_H
#define CENTRALITYOUTPUTMANAGER_H

#include <string>
#include <vector>

#include "TTree.h"

#include <fun4all/Fun4AllOutputManager.h>

namespace HepMC {
  class IO_GenEvent;
}

class PHCompositeNode;

class CentralityOutputManager: public Fun4AllOutputManager {
 public:
  CentralityOutputManager(std::string const& name);
  ~CentralityOutputManager();

  void write_bbc_hits();
  void write_epd_hits();

  int Write(PHCompositeNode*);

 private:
  TTree* _t;

  float _b;
  int _npart;
  int _ncoll;

  float _epd_sum_e;
  float _bbc_sum_e;

  std::string _name;

  bool _write_bbc;
  bool _write_epd;
};

#endif /* CENTRALITYOUTPUTMANAGER_H */
