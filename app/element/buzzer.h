#ifndef BUZZER_H
#define BUZZER_H

#include "graphicelement.h"

#include <QSound>
#include <QSoundEffect>

class Buzzer : public GraphicElement {
public:
  explicit Buzzer( QGraphicsItem *parent = nullptr );
  virtual ~Buzzer( ) override = default;

public:
  /* GraphicElement interface */
  virtual ElementType elementType( ) override {
    return( ElementType::BUZZER );
  }
  virtual ElementGroup elementGroup( ) override {
    return( ElementGroup::OUTPUT );
  }
  void refresh( ) override;

  void setAudio( QString note ) override;
  QString getAudio( ) const override;

  void mute( bool _mute = true );

private:
  void playbuzzer( );
  void stopbuzzer( );
  int play;
  QSoundEffect m_audio;
  QString m_note;
  // GraphicElement interface
public:
  void save( QDataStream &ds ) const override;
  void load( QDataStream &ds, QMap< quint64, QNEPort* > &portMap, double version ) override;
};

#endif // BUZZER_H
