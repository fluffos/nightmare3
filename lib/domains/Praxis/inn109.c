//Lassondra@Nightmare
inherit "std/room";

void create() {
  ::create();
set_short("the honeymoon suite" );
set_properties( ([ "no castle" : 1, "light" : 2 ]) );
set_long( "The honeymoon suite is a very luxurious room. A large "
"bed, made up with a emerald green silk comforter stands in themiddle "
"of the room. The carpet beneath your feet is a dark beige colour, "
"almost a perfect match for the wooden hue of the nightstand and "
"bureau. Large double doors with golden handles lead into and "
"out of the suite." );
set_items( ([ "suite" : "The suite is a very large and expensive "
"looking room at the Nightmare Inn.",
"room" : "The room is one of the honeymoon suites at the "
"Nightmare Inn.",
"carpet" : "The carpet is very thick and a deep hue of beige.",
"bureau" : "The bureau has been polished many times, you can tell "
"by howit gleems in the light.",
"nightstand" : "The nightstands have been placed on either sides "
"iof the bed.",
({ "doors", "door" }) : "The large opaken doors are a very "
"deep shade of mahogany.",
({ "handles", "handle" }) : "The handles on the double doors "
"are golden."
]) );
set_exits( ([ "leave" : "/domains/Praxis/hall5.c" ]) );
  }
