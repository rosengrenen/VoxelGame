#include "pch.h"
#include "Block.h"

#include <bitset>

Block::Block() : Block(255, 255, 255, 255) {}

Block::Block(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    SetRed(r);
    SetGreen(g);
    SetBlue(b);
    SetAlpha(a);
}

unsigned char Block::GetRed() const
{
    return m_red;
}

unsigned char Block::GetGreen() const
{
    return m_green;
}

unsigned char Block::GetBlue() const
{
    return m_blue;
}

unsigned char Block::GetAlpha() const
{
    return m_alpha;
}

void Block::SetRed(unsigned char red)
{
    m_red = red;
}

void Block::SetGreen(unsigned char green)
{
    m_green = green;
}

void Block::SetBlue(unsigned char blue)
{
    m_blue = blue;
}

void Block::SetAlpha(unsigned char alpha)
{
    m_alpha = alpha;
}

bool Block::IsActive() const
{
    return GetAlpha() != 255;
}
