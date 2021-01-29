using System;
using System.Windows.Forms;
using Engine;

namespace Example
{

public class Dog
{
    private int m_barks;

    public int Barks
    {
        get { return m_barks; }
        set { m_barks = value; }
    }

    public Dog()
    {

    }

    public void bark()
    {
        m_barks++;
    }

    static public float pow(float x, float y)
    {
        return (float)Math.Pow(x, y);
    }

    
}

}