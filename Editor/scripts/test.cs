using System;
using System.Windows.Forms;

namespace Example
{

public class Dog
{
    public Dog()
    {
        //Console.WriteLine("New dog object constructed!");
        Engine.Test tst;
    }

    static public void bark()
    {
        Console.WriteLine("Bark!");
    }
}

}