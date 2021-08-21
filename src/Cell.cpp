#include "Cell.h"

Cell::Cell(unsigned value)
: content(static_cast<Content>(value))
{
}

Cell::~Cell()
{
    //dtor
}

void Cell::setContent(unsigned value)
{
    content = static_cast<Content>(value);
}

void Cell::setContent(Content value)
{
    content = value;
}

