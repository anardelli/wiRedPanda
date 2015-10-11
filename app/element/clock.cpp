#include "clock.h"

#include <QDebug>

bool Clock::reset = false;

Clock::Clock(QGraphicsItem * parent) : GraphicElement(0,0,1,1,parent) {
  setOutputsOnTop(false);
  setRotatable(false);
  setPixmap(QPixmap(":/input/clock0.png"));
//  connect(&timer,&QTimer::timeout,this,&Clock::updateClock);
  setFrequency(1.0);
  setHasFrequency(true);
  on = false;
  Clock::reset = true;
  setHasLabel(true);
  setObjectName("Clock");
}

Clock::~Clock() {
}

void Clock::updateClock() {
  elapsed = 0;
  if(on) {
    on = false;
    setPixmap(QPixmap(":/input/clock0.png"));
  } else {
    on = true;
    setPixmap(QPixmap(":/input/clock1.png"));
  }
  outputs().first()->setValue(on);
}

void Clock::save(QDataStream & ds) {
  GraphicElement::save(ds);
  ds << frequency();
}

void Clock::load(QDataStream & ds, QMap<quint64, QNEPort *> & portMap, double version) {
  GraphicElement::load(ds,portMap,version);
  if(version >= 1.1) {
    float freq;
    ds >> freq;
    setFrequency(freq);
  }
}

float Clock::frequency() {
  return m_frequency;
}

void Clock::setFrequency(float freq) {
//  qDebug() << "Clock frequency set to " << freq;
  if( freq > 0 && m_frequency != freq) {
    m_frequency = freq;
    interval = static_cast< int >(frequencyMultiplier/freq);
    elapsed = 0;
    Clock::reset = true;
    //    timer.start( static_cast< int >(1000.0/freq) );
  }
}

void Clock::updateLogic() {
  if(!disabled()){
    elapsed++;
    setChanged(true);
    if( (elapsed%interval) == 0) {
      updateClock();
    }
  }
}

void Clock::resetClock() {
  on = true;
  updateClock();
  elapsed = 0;
}