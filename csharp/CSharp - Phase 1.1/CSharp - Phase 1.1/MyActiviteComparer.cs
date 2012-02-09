using System;
using System.Collections.Generic;
using System.Text;

namespace ConsoleApp
{
    class MyActiviteComparer : IComparer<Activite>
    {
        public int Compare(Activite o1, Activite o2)
        {
            return o1.Lieu.CompareTo(o2.Lieu);
        }
    }
}
