#ifndef INPUTVCC_H
#define INPUTVCC_H

#include "graphicelement.h"

class InputVcc : public GraphicElement {
public:
  explicit InputVcc( QGraphicsItem *parent = nullptr );
  virtual ~InputVcc( ) override = default;
  /* GraphicElement interface */
public:
  virtual ElementType elementType( ) override {
    return( ElementType::VCC );
  }
  virtual ElementGroup elementGroup( ) override {
    return( ElementGroup::INPUT );
  }
  void updateLogic( );
};

#endif /* INPUTVCC_H */
