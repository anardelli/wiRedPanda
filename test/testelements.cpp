#include "testelements.h"
#include <QDebug>

#include <demux.h>
#include <dlatch.h>
#include <iostream>
#include <jkflipflop.h>
#include <jklatch.h>
#include <mux.h>
#include <srflipflop.h>
#include <tflipflop.h>
#include <tlatch.h>

#include "and.h"
#include "dflipflop.h"
#include "inputgnd.h"
#include "inputvcc.h"
#include "or.h"

TestElements::TestElements( QObject *parent ) : QObject( parent ) {

}

void TestElements::init( ) {
  /* Creating connections */
  for( size_t i = 0; i < conn.size( ); ++i ) {
    conn.at( i ) = new QNEConnection( );
    sw.at( i ) = new InputSwitch( );
    conn.at( i )->setPort1( sw.at( i )->outputs( ).first( ) );
  }
}


void TestElements::cleanup( ) {
  for( size_t i = 0; i < conn.size( ); ++i ) {
    delete sw.at( i );
  }
}


void TestElements::testAnd( ) {
  And elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 2 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 1 );
  QCOMPARE( elm.minInputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::AND );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) );
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) );

  std::array< std::array< int, 3 >, 4 > truthTable = {
    {
      { { 1, 1, 1 } },
      { { 1, 0, 0 } },
      { { 0, 1, 0 } },
      { { 0, 0, 0 } }
    }
  };
  for( int test = 0; test < 4; ++test ) {
    sw.at( 0 )->setOn( truthTable.at( test ).at( 0 ) );
    sw.at( 1 )->setOn( truthTable.at( test ).at( 1 ) );
    sw.at( 0 )->updateLogic( );
    sw.at( 1 )->updateLogic( );
    elm.updateLogic( );
    QCOMPARE( ( int ) elm.outputs( ).front( )->value( ), truthTable.at( test ).at( 2 ) );
  }
}

void TestElements::testOr( ) {
  Or elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 2 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 1 );
  QCOMPARE( elm.minInputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::OR );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) );
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) );

  std::array< std::array< int, 3 >, 4 > truthTable = {
    {
      { { 1, 1, 1 } },
      { { 1, 0, 1 } },
      { { 0, 1, 1 } },
      { { 0, 0, 0 } }
    }
  };
  for( int test = 0; test < 4; ++test ) {
    sw.at( 0 )->setOn( truthTable.at( test ).at( 0 ) );
    sw.at( 1 )->setOn( truthTable.at( test ).at( 1 ) );
    sw.at( 0 )->updateLogic( );
    sw.at( 1 )->updateLogic( );
    elm.updateLogic( );
    QCOMPARE( ( int ) elm.outputs( ).front( )->value( ), truthTable.at( test ).at( 2 ) );
  }
}

void TestElements::testVCC( ) {
  InputVcc vcc;
  QCOMPARE( ( int ) vcc.outputs( ).front( )->value( ), 1 );
}


void TestElements::testGND( ) {
  InputGnd gnd;
  QCOMPARE( ( int ) gnd.outputs( ).front( )->value( ), 0 );

}

void TestElements::testMux( ) {
  Mux elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 3 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 1 );
  QCOMPARE( elm.minInputSz( ), 3 );
  QCOMPARE( elm.elementType( ), ElementType::MUX );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) );
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) );
  conn.at( 2 )->setPort2( elm.inputs( ).at( 2 ) );

  std::array< std::array< int, 4 >, 8 > truthTable = {
    {
      { { 0, 0, 0, 0 } },
      { { 0, 0, 1, 0 } },
      { { 0, 1, 0, 0 } },
      { { 0, 1, 1, 1 } },
      { { 1, 0, 0, 1 } },
      { { 1, 0, 1, 0 } },
      { { 1, 1, 0, 1 } },
      { { 1, 1, 1, 1 } },
    }
  };
  for( int test = 0; test < 4; ++test ) {
    sw.at( 0 )->setOn( truthTable.at( test ).at( 0 ) );
    sw.at( 1 )->setOn( truthTable.at( test ).at( 1 ) );
    sw.at( 2 )->setOn( truthTable.at( test ).at( 2 ) );
    sw.at( 0 )->updateLogic( );
    sw.at( 1 )->updateLogic( );
    sw.at( 2 )->updateLogic( );
    elm.updateLogic( );
    QCOMPARE( ( int ) elm.outputs( ).front( )->value( ), truthTable.at( test ).at( 3 ) );
  }
}

void TestElements::testDemux( ) {
  Demux elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 2 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 2 );
  QCOMPARE( elm.minInputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::DEMUX );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) );
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) );

  std::array< std::array< int, 4 >, 8 > truthTable = {
    {
      /*  i  S  o0 o1 */
      { { 0, 0, 0, 0 } },
      { { 0, 1, 0, 0 } },
      { { 1, 0, 1, 0 } },
      { { 1, 1, 0, 1 } },
    }
  };
  for( int test = 0; test < 4; ++test ) {
    sw.at( 0 )->setOn( truthTable.at( test ).at( 0 ) );
    sw.at( 1 )->setOn( truthTable.at( test ).at( 1 ) );
    sw.at( 0 )->updateLogic( );
    sw.at( 1 )->updateLogic( );
    elm.updateLogic( );
    QCOMPARE( ( int ) elm.outputs( ).at( 0 )->value( ), truthTable.at( test ).at( 2 ) );
    QCOMPARE( ( int ) elm.outputs( ).at( 1 )->value( ), truthTable.at( test ).at( 3 ) );
  }
}

void TestElements::testDFlipFlop( ) {
  DFlipFlop elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 4 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 2 );
  QCOMPARE( elm.minInputSz( ), 4 );
  QCOMPARE( elm.maxInputSz( ), 4 );
  QCOMPARE( elm.minOutputSz( ), 2 );
  QCOMPARE( elm.maxOutputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::DFLIPFLOP );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) );
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) );
  conn.at( 2 )->setPort2( elm.inputs( ).at( 2 ) );
  conn.at( 3 )->setPort2( elm.inputs( ).at( 3 ) );

  std::array< std::array< int, 7 >, 7 > truthTable = {
    {
      /* L  D  C  p  c  Q ~Q */
      { { 0, 0, 1, 1, 1, 0, 1 } }, /* Clk up and D = 0 */
      { { 0, 1, 1, 1, 1, 1, 0 } }, /* Clk up and D = 1 */

      { { 0, 0, 0, 0, 1, 1, 0 } }, /* Preset = false */
      { { 0, 0, 1, 1, 0, 0, 1 } }, /* Clear = false */
      { { 0, 0, 1, 0, 0, 1, 1 } }, /* Clear and Preset = false */

      { { 1, 0, 0, 1, 1, 1, 1 } }, /* Clk dwn and D = 0 ( must mantain current state )*/
      { { 1, 1, 0, 1, 1, 1, 1 } }, /* Clk dwn and D = 1 ( must mantain current state )*/
    }
  };
  for( size_t test = 0; test < truthTable.size( ); ++test ) {
    elm.outputs( ).at( 0 )->setValue( truthTable.at( test ).at( 0 ) );
    sw.at( 0 )->setOn( truthTable.at( test ).at( 1 ) ); /* DATA */
    sw.at( 1 )->setOn( truthTable.at( test ).at( 0 ) ); /* CLK */
    sw.at( 2 )->setOn( false ); /* PRST */
    sw.at( 3 )->setOn( false ); /* CLR */
    for( int port = 0; port < 4; ++port ) {
      sw.at( port )->updateLogic( );
    }
    elm.updateLogic( );
/*    std::cout << ( int ) elm.outputs( ).at( 0 )->value( ) << " -> "; */
    for( int port = 0; port < 4; ++port ) {
      sw.at( port )->setOn( truthTable.at( test ).at( port + 1 ) );
      sw.at( port )->updateLogic( );
/*      std::cout << truthTable.at(test).at( port + 1 ) << " "; */
    }
    elm.updateLogic( );
/*    std::cout << "-> " << ( int ) elm.outputs( ).at( 0 )->value( ) << std::endl; */

    QCOMPARE( ( int ) elm.outputs( ).at( 0 )->value( ), truthTable.at( test ).at( 5 ) );
    QCOMPARE( ( int ) elm.outputs( ).at( 1 )->value( ), truthTable.at( test ).at( 6 ) );
  }
//  elm.inputs( ).at( 2 )->disconnect( conn.at( 2 ) );
//  elm.inputs( ).at( 3 )->disconnect( conn.at( 3 ) );
  delete conn.at( 2 );
  delete conn.at( 3 );
  QCOMPARE( ( int ) elm.inputs( ).at( 2 )->value( ), 1 );
  QCOMPARE( ( int ) elm.inputs( ).at( 3 )->value( ), 1 );
  elm.updateLogic( );
  QVERIFY( ( int ) elm.outputs( ).at( 0 )->value( ) != -1 );
  QVERIFY( ( int ) elm.outputs( ).at( 1 )->value( ) != -1 );
}

void TestElements::testDLatch( ) {
  DLatch elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 2 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 2 );
  QCOMPARE( elm.minInputSz( ), 2 );
  QCOMPARE( elm.maxInputSz( ), 2 );
  QCOMPARE( elm.minOutputSz( ), 2 );
  QCOMPARE( elm.maxOutputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::DLATCH );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) );
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) );

  std::array< std::array< int, 4 >, 6 > truthTable = {
    {
      /*D  E  Q  A */
      { { 0, 0, 0, 0 } },
      { { 0, 0, 1, 1 } },
      { { 1, 0, 0, 0 } },
      { { 1, 0, 1, 1 } },
      { { 0, 1, 0, 1 } },
      { { 1, 1, 1, 0 } },
    }
  };
  for( size_t test = 0; test < truthTable.size( ); ++test ) {
    elm.outputs( ).at( 0 )->setValue( truthTable.at( test ).at( 3 ) );
    elm.outputs( ).at( 1 )->setValue( !truthTable.at( test ).at( 3 ) );
    for( int port = 0; port < 2; ++port ) {
      sw.at( port )->setOn( truthTable.at( test ).at( port ) );
      sw.at( port )->updateLogic( );
/*      std::cout << truthTable.at(test).at( port ) << " "; */
    }
    elm.updateLogic( );
/*    std::cout << "-> " << ( int ) elm.outputs( ).at( 0 )->value( ) << std::endl; */

    QCOMPARE( ( int ) elm.outputs( ).at( 0 )->value( ), truthTable.at( test ).at( 2 ) );
    QCOMPARE( ( int ) elm.outputs( ).at( 1 )->value( ), ( int ) !truthTable.at( test ).at( 2 ) );
  }
}

void TestElements::testJKFlipFlop( ) {
  JKFlipFlop elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 5 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 2 );
  QCOMPARE( elm.minInputSz( ), 5 );
  QCOMPARE( elm.maxInputSz( ), 5 );
  QCOMPARE( elm.minOutputSz( ), 2 );
  QCOMPARE( elm.maxOutputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::JKFLIPFLOP );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) ); /* J */
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) ); /* Clk */
  conn.at( 2 )->setPort2( elm.inputs( ).at( 2 ) ); /* K */
  conn.at( 3 )->setPort2( elm.inputs( ).at( 3 ) ); /* Prst */
  conn.at( 4 )->setPort2( elm.inputs( ).at( 4 ) ); /* Clr */

  std::array< std::array< int, 9 >, 13 > truthTable = {
    {
      /*    L  J  C  K  p  c  Q Q A */
      { { 0, 0, 0, 0, 0, 1, 1, 0, 0 } }, /* Preset = false */
      { { 0, 0, 0, 0, 1, 0, 0, 1, 0 } }, /* Clear = false */
      { { 0, 0, 0, 1, 0, 0, 1, 1, 0 } }, /* Clear and Preset = false */
      { { 1, 1, 0, 0, 1, 1, 0, 1, 0 } }, /* Clk dwn and J = 0 ( must mantain current state )*/
      { { 1, 1, 0, 0, 1, 1, 0, 1, 0 } }, /* Clk dwn and J = 1 ( must mantain current state )*/
      { { 0, 0, 1, 0, 1, 1, 1, 0, 0 } }, /* Clk up J = 0 K = 0 ( must swap Q and ~Q )*/
      { { 0, 0, 1, 0, 1, 1, 0, 1, 1 } }, /* Clk up J = 0 K = 0 ( must swap Q and ~Q )*/
      { { 0, 0, 1, 1, 1, 1, 0, 1, 0 } }, /* Clk up J = 0 K = 1 */
      { { 0, 0, 1, 1, 1, 1, 0, 1, 1 } }, /* Clk up J = 0 K = 1 */
      { { 0, 1, 1, 0, 1, 1, 1, 0, 0 } }, /* Clk up J = 1 K = 0 */
      { { 0, 1, 1, 0, 1, 1, 1, 0, 1 } }, /* Clk up J = 1 K = 0 */
      { { 0, 1, 1, 1, 1, 1, 0, 1, 0 } }, /* Clk up J = 1 K = 0 */
      { { 0, 1, 1, 1, 1, 1, 1, 0, 1 } }, /* Clk up J = 1 K = 0 */

    }
  };
  elm.updateLogic( );
  for( size_t test = 0; test < truthTable.size( ); ++test ) {
    sw.at( 0 )->setOn( false );
    sw.at( 1 )->setOn( truthTable.at( test ).at( 0 ) );
    sw.at( 2 )->setOn( false );
    sw.at( 3 )->setOn( false );
    sw.at( 4 )->setOn( false );
    for( int port = 0; port < 5; ++port ) {
      sw.at( port )->updateLogic( );
    }
    elm.updateLogic( );
    elm.outputs( ).at( 0 )->setValue( truthTable.at( test ).at( 8 ) );
    elm.outputs( ).at( 1 )->setValue( !truthTable.at( test ).at( 8 ) );
/*    std::cout << ( int ) elm.outputs( ).at( 0 )->value( ) << " -> "; */
    for( int port = 0; port < 5; ++port ) {
      sw.at( port )->setOn( truthTable.at( test ).at( port + 1 ) );
      sw.at( port )->updateLogic( );
/*      std::cout << truthTable.at(test).at( port + 1 ) << " "; */
    }
    elm.updateLogic( );
/*    std::cout << "-> " << ( int ) elm.outputs( ).at( 0 )->value( ) << std::endl; */

    QCOMPARE( ( int ) elm.outputs( ).at( 0 )->value( ), truthTable.at( test ).at( 6 ) );
    QCOMPARE( ( int ) elm.outputs( ).at( 1 )->value( ), truthTable.at( test ).at( 7 ) );
  }
//  elm.inputs( ).at( 3 )->disconnect( conn.at( 3 ) );
//  elm.inputs( ).at( 4 )->disconnect( conn.at( 4 ) );

  delete conn.at(3);
  delete conn.at(4);

  QCOMPARE( ( int ) elm.inputs( ).at( 3 )->value( ), 1 );
  QCOMPARE( ( int ) elm.inputs( ).at( 4 )->value( ), 1 );
  elm.updateLogic( );
  QVERIFY( ( int ) elm.outputs( ).at( 0 )->value( ) != -1 );
  QVERIFY( ( int ) elm.outputs( ).at( 1 )->value( ) != -1 );
}

void TestElements::testJKLatch( ) {
  JKLatch elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 2 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 2 );
  QCOMPARE( elm.minInputSz( ), 2 );
  QCOMPARE( elm.maxInputSz( ), 2 );
  QCOMPARE( elm.minOutputSz( ), 2 );
  QCOMPARE( elm.maxOutputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::JKLATCH );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) );
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) );

  std::array< std::array< int, 4 >, 8 > truthTable = {
    {
      /*    J  K  Q  A */
      { { 0, 0, 0, 0 } }, /* No change */
      { { 0, 0, 1, 1 } }, /* No change */
      { { 0, 1, 0, 0 } }, /* Reset */
      { { 0, 1, 0, 1 } }, /* Reset */
      { { 1, 0, 1, 0 } }, /* Set */
      { { 1, 0, 1, 1 } }, /* Set */
      { { 1, 1, 1, 0 } }, /* Toggle */
      { { 1, 1, 0, 1 } }, /* Toggle */
    }
  };
  for( size_t test = 0; test < truthTable.size( ); ++test ) {
    elm.outputs( ).at( 0 )->setValue( truthTable.at( test ).at( 3 ) );
    elm.outputs( ).at( 1 )->setValue( !truthTable.at( test ).at( 3 ) );
    for( int port = 0; port < 2; ++port ) {
      sw.at( port )->setOn( truthTable.at( test ).at( port ) );
      sw.at( port )->updateLogic( );
/*      std::cout << truthTable.at(test).at( port ) << " "; */
    }
    elm.updateLogic( );
/*    std::cout << "-> " << ( int ) elm.outputs( ).at( 0 )->value( ) << std::endl; */
    QCOMPARE( ( int ) elm.outputs( ).at( 0 )->value( ), truthTable.at( test ).at( 2 ) );
    QCOMPARE( ( int ) elm.outputs( ).at( 1 )->value( ), ( int ) !truthTable.at( test ).at( 2 ) );
  }
}

void TestElements::testSRFlipFlop( ) {
  SRFlipFlop elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 3 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 2 );
  QCOMPARE( elm.minInputSz( ), 3 );
  QCOMPARE( elm.maxInputSz( ), 3 );
  QCOMPARE( elm.minOutputSz( ), 2 );
  QCOMPARE( elm.maxOutputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::SRFLIPFLOP );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) ); /* S */
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) ); /* Clk */
  conn.at( 2 )->setPort2( elm.inputs( ).at( 2 ) ); /* R */

  std::array< std::array< int, 7 >, 20 > truthTable = {
    {
      /*   L   S  C R  Q  Q  A */
      { { 0, 0, 0, 0, 0, 1, 0 } }, /* No change */
      { { 0, 0, 0, 0, 1, 0, 1 } }, /* No change */
      { { 0, 1, 0, 0, 0, 1, 0 } }, /* No change */
      { { 0, 1, 0, 0, 1, 0, 1 } }, /* No change */

      { { 0, 0, 0, 1, 0, 1, 0 } }, /* No change */
      { { 0, 0, 0, 1, 1, 0, 1 } }, /* No change */
      { { 1, 0, 1, 0, 0, 1, 0 } }, /* No change */
      { { 1, 0, 1, 0, 1, 0, 1 } }, /* No change */

      { { 1, 1, 1, 0, 0, 1, 0 } }, /* No change */
      { { 1, 1, 1, 0, 1, 0, 1 } }, /* No change */
      { { 1, 0, 1, 1, 0, 1, 0 } }, /* No change */
      { { 1, 0, 1, 1, 1, 0, 1 } }, /* No change */

      { { 0, 0, 1, 0, 0, 1, 0 } }, /* No change */
      { { 0, 0, 1, 0, 1, 0, 1 } }, /* No change */
      { { 0, 0, 1, 1, 0, 1, 0 } }, /* Q = 0 */
      { { 0, 0, 1, 1, 0, 1, 1 } }, /* Q = 0 */

      { { 0, 1, 1, 0, 1, 0, 0 } }, /* Q = 1 */
      { { 0, 1, 1, 0, 1, 0, 1 } }, /* Q = 1 */
      { { 0, 1, 1, 1, 1, 1, 0 } }, /* Not permitted */
      { { 0, 1, 1, 1, 1, 1, 1 } }, /* Not permitted */
    }
  };
  elm.updateLogic( );
  for( size_t test = 0; test < truthTable.size( ); ++test ) {
    sw.at( 0 )->setOn( false );
    sw.at( 1 )->setOn( truthTable.at( test ).at( 0 ) );
    sw.at( 2 )->setOn( false );
    for( int port = 0; port < 3; ++port ) {
      sw.at( port )->updateLogic( );
    }
    elm.updateLogic( );
    elm.outputs( ).at( 0 )->setValue( truthTable.at( test ).at( 6 ) );
    elm.outputs( ).at( 1 )->setValue( !truthTable.at( test ).at( 6 ) );
/*    std::cout << ( int ) elm.outputs( ).at( 0 )->value( ) << " " << ( int ) elm.outputs( ).at( 1 )->value( ) << " ->
 * "; */
    for( int port = 0; port < 3; ++port ) {
      sw.at( port )->setOn( truthTable.at( test ).at( port + 1 ) );
      sw.at( port )->updateLogic( );
/*      std::cout << truthTable.at(test).at( port + 1 ) << " "; */
    }
    elm.updateLogic( );
/*
 *    std::cout << "-> " << ( int ) elm.outputs( ).at( 0 )->value( ) << " " << ( int ) elm.outputs( ).at( 1 )->value( )
 * <<
 *      std::endl;
 */

    QCOMPARE( ( int ) elm.outputs( ).at( 0 )->value( ), truthTable.at( test ).at( 4 ) );
    QCOMPARE( ( int ) elm.outputs( ).at( 1 )->value( ), truthTable.at( test ).at( 5 ) );
  }
}

void TestElements::testTFlipFlop( ) {
  TFlipFlop elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 4 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 2 );
  QCOMPARE( elm.minInputSz( ), 4 );
  QCOMPARE( elm.maxInputSz( ), 4 );
  QCOMPARE( elm.minOutputSz( ), 2 );
  QCOMPARE( elm.maxOutputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::TFLIPFLOP );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) ); /* T */
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) ); /* Clk */
  conn.at( 2 )->setPort2( elm.inputs( ).at( 2 ) ); /* Prst */
  conn.at( 3 )->setPort2( elm.inputs( ).at( 3 ) ); /* Clr */

  std::array< std::array< int, 8 >, 11 > truthTable = {
    {
      /*    L  T  C  p  c  Q Q A */
      { { 0, 0, 0, 1, 1, 0, 1, 0 } }, /* No change */
      { { 0, 1, 0, 1, 1, 0, 1, 0 } }, /* No change */
      { { 0, 0, 0, 1, 1, 1, 0, 1 } }, /* No change */
      { { 0, 1, 0, 1, 1, 1, 0, 1 } }, /* No change */

      { { 0, 0, 1, 1, 1, 0, 1, 0 } }, /* No change */
      { { 0, 0, 1, 1, 1, 1, 0, 1 } }, /* No change */

      { { 0, 1, 1, 1, 1, 1, 0, 0 } }, /* Toggle */
      { { 0, 1, 1, 1, 1, 0, 1, 1 } }, /* Toggle */


      { { 0, 0, 0, 0, 1, 1, 0, 0 } }, /* Preset = false */
      { { 0, 0, 0, 1, 0, 0, 1, 1 } }, /* Clear = false */
      { { 0, 0, 0, 0, 0, 1, 1, 1 } }, /* Clear and Preset = false */

      /* Test Prst and clr */
    }
  };
  elm.updateLogic( );
  for( size_t test = 0; test < truthTable.size( ); ++test ) {
    sw.at( 0 )->setOn( false );
    sw.at( 1 )->setOn( truthTable.at( test ).at( 0 ) );
    sw.at( 2 )->setOn( false );
    sw.at( 3 )->setOn( false );
    for( int port = 0; port < 4; ++port ) {
      sw.at( port )->updateLogic( );
    }
    elm.updateLogic( );
    elm.outputs( ).at( 0 )->setValue( truthTable.at( test ).at( 7 ) );
    elm.outputs( ).at( 1 )->setValue( !truthTable.at( test ).at( 7 ) );
/*    std::cout << ( int ) elm.outputs( ).at( 0 )->value( ) << " " << ( int ) elm.outputs( ).at( 1 )->value( ) << " ->
 * "; */
    for( int port = 0; port < 4; ++port ) {
      sw.at( port )->setOn( truthTable.at( test ).at( port + 1 ) );
      sw.at( port )->updateLogic( );
/*      std::cout << truthTable.at(test).at( port + 1 ) << " "; */
    }
    elm.updateLogic( );
/*    std::cout << "-> " << ( int ) elm.outputs( ).at( 0 )->value( ) << " " << ( int ) elm.outputs( ).at( 1 )->value( )
 * << std::endl; */

    QCOMPARE( ( int ) elm.outputs( ).at( 0 )->value( ), truthTable.at( test ).at( 5 ) );
    QCOMPARE( ( int ) elm.outputs( ).at( 1 )->value( ), truthTable.at( test ).at( 6 ) );
  }
//  elm.inputs( ).at( 2 )->disconnect( conn.at( 2 ) );
//  elm.inputs( ).at( 3 )->disconnect( conn.at( 3 ) );


  delete conn.at(2);
  delete conn.at(3);


  QCOMPARE( ( int ) elm.inputs( ).at( 2 )->value( ), 1 );
  QCOMPARE( ( int ) elm.inputs( ).at( 3 )->value( ), 1 );
  elm.updateLogic( );
  QVERIFY( ( int ) elm.outputs( ).at( 0 )->value( ) != -1 );
  QVERIFY( ( int ) elm.outputs( ).at( 1 )->value( ) != -1 );
}

void TestElements::testTLatch( ) {
  TLatch elm;
  QCOMPARE( elm.inputSize( ), elm.inputs( ).size( ) );
  QCOMPARE( elm.inputSize( ), 2 );
  QCOMPARE( elm.outputSize( ), elm.outputs( ).size( ) );
  QCOMPARE( elm.outputSize( ), 2 );
  QCOMPARE( elm.minInputSz( ), 2 );
  QCOMPARE( elm.maxInputSz( ), 2 );
  QCOMPARE( elm.minOutputSz( ), 2 );
  QCOMPARE( elm.maxOutputSz( ), 2 );
  QCOMPARE( elm.elementType( ), ElementType::TLATCH );
  conn.at( 0 )->setPort2( elm.inputs( ).at( 0 ) );
  conn.at( 1 )->setPort2( elm.inputs( ).at( 1 ) );

  std::array< std::array< int, 4 >, 6 > truthTable = {
    {
      /*T  E  Q  A */
      { { 0, 0, 0, 0 } }, /* No change */
      { { 0, 0, 1, 1 } }, /* No change */
      { { 1, 1, 1, 0 } },
      { { 1, 1, 0, 1 } },
      { { 0, 1, 1, 1 } },
      { { 0, 1, 0, 0 } },
    }
  };
  for( size_t test = 0; test < truthTable.size( ); ++test ) {
    elm.outputs( ).at( 0 )->setValue( truthTable.at( test ).at( 3 ) );
    elm.outputs( ).at( 1 )->setValue( !truthTable.at( test ).at( 3 ) );
    for( int port = 0; port < 2; ++port ) {
      sw.at( port )->setOn( truthTable.at( test ).at( port ) );
      sw.at( port )->updateLogic( );
/*      std::cout << truthTable.at(test).at( port ) << " "; */
    }
    elm.updateLogic( );
/*    std::cout << "-> " << ( int ) elm.outputs( ).at( 0 )->value( ) << std::endl; */

    QCOMPARE( ( int ) elm.outputs( ).at( 0 )->value( ), truthTable.at( test ).at( 2 ) );
    QCOMPARE( ( int ) elm.outputs( ).at( 1 )->value( ), ( int ) !truthTable.at( test ).at( 2 ) );
  }
}