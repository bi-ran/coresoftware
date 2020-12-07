#include "CentralityOutputManager.h"

#include <ffaobjects/EventHeader.h>
#include <frog/FROG.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/PHTFileServer.h>
#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>
#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4Hit.h>

#include <HepMC/IO_GenEvent.h>

#include <iostream>

CentralityOutputManager::CentralityOutputManager(std::string const& name)
  : Fun4AllOutputManager("EPDOutput") {
  _name = name;

  _write_bbc = false;
  _write_epd = false;

  _b = -1;
  _npart = -1;
  _ncoll = -1;

  _epd_sum_e = -1;
  _bbc_sum_e = -1;

  PHTFileServer::get().open(_name, "RECREATE");

  _t = new TTree("HITS", "");

  _t->Branch("b", &_b);
  _t->Branch("npart", &_npart);
  _t->Branch("ncoll", &_ncoll);
}

CentralityOutputManager::~CentralityOutputManager()
{
  PHTFileServer::get().cd(_name);

  _t->Write("", TObject::kOverwrite);

  PHTFileServer::get().close();
}

int CentralityOutputManager::Write(PHCompositeNode *topNode)
{
  auto event = findNode::getClass<EventHeader>(topNode, "EventHeader");

  _b = event->get_ImpactParameter();
  _npart = event->get_npart();
  _ncoll = event->get_ncoll();

  if (_write_bbc) {
    _bbc_sum_e = 0;

    auto bhits = findNode::getClass<PHG4HitContainer>(topNode, "G4HIT_BBC");

    if (bhits != nullptr) {
      auto brange = bhits->getHits();
      for (auto it = brange.first; it != brange.second; ++it)
        _bbc_sum_e += it->second->get_edep();
    }
  }

  if (_write_epd) {
    _epd_sum_e = 0;

    auto ehits = findNode::getClass<PHG4HitContainer>(topNode, "G4HIT_EPD");

    if (ehits != nullptr) {
      auto erange = ehits->getHits();
      for (auto it = erange.first; it != erange.second; ++it)
        _epd_sum_e += it->second->get_edep();
    }
  }

  _t->Fill();

  return 0;
}

void CentralityOutputManager::write_bbc_hits() {
  _write_bbc = true;

  _t->Branch("bbc_sum_e", &_bbc_sum_e);
}

void CentralityOutputManager::write_epd_hits() {
  _write_epd = true;

  _t->Branch("epd_sum_e", &_epd_sum_e);
}
