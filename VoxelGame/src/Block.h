#pragma once

class Block
{
public:
    /* Public methods */
    Block();
    Block(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    unsigned char GetRed() const;
    unsigned char GetGreen() const;
    unsigned char GetBlue() const;
    unsigned char GetAlpha() const;

    void SetRed(unsigned char red);
    void SetGreen(unsigned char green);
    void SetBlue(unsigned char blue);
    void SetAlpha(unsigned char alpha);

    bool IsActive() const;

private:
    /* Private members */
    unsigned char m_red;
    unsigned char m_green;
    unsigned char m_blue;
    unsigned char m_alpha;
};
