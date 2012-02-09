using System;
using System.Collections.Generic;
using System.Text;

namespace ConsoleApp
{
    class Activite : IComparable<Activite>
    {
        private string _name;
        private DateTime _debut;
        private DateTime _fin;
        private string _location;
        private string _description;

        public string Nom
        {
            get { return _name; }
            set { _name = value; }
        }

        public DateTime DateDebut
        {
            get { return _debut; }
            set { _debut = value; }
        }


        // Test 6
        public string Lieu
        {
            get { return _location; }
            set { _location = value; }
        }
        //

        public Activite(string name, DateTime debut, DateTime fin, string location, string description)
        {
            _name        = name;
            _debut       = debut;
            _fin         = fin;
            _location    = location;
            _description = description;
        }

        public Activite(string name, DateTime debut, string dummy, string location, string description)
            : this(name, debut, new DateTime(), location, description) { }

        public Activite(string name) : this(name, new DateTime(), new DateTime(), "", "") {}
        public Activite() : this("New Event", new DateTime(), new DateTime(), "", "") {}

        public void SetActivites(DateTime debut, DateTime fin, string location, string description) {
            _debut = debut;
            _fin   = fin;
            _location    = location;
            _description = description;
        }

        public override string ToString()
        {
            return _name;
        }

        public string ToString(string type, string wat)
        {
            switch (type)
            {
                case "Resume":
                    return "\n" + _debut + " -> " + _fin + "\n" + _name;

                case "Detail":
                    return "Le " + _debut + " -> " + _fin + ": " + _name + "\nOù: " + _location + "\n" + _description + "\n";

                default:
                    return _name;
            }
            
        }

        // Test 2
        public override bool Equals(object o)
        {
            Activite temp = (Activite)o;
            return (temp.Nom == this._name);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        // Test 3
        public static bool operator == (Activite o1, Activite o2)
        {
            return o1.Equals(o2);
        }

        public static bool operator != (Activite o1, Activite o2)
        {
            return !(o1.Equals(o2));
        }

        // Test 5
        public static int ComparisonDateDebut(Activite o1, Activite o2)
        {
            if (o1.DateDebut == o2.DateDebut)
                return 0;

            else if (o1.DateDebut < o2.DateDebut)
                return -1;

            else return 1;
        }

        #region IComparable<Activite> Membres

        public int CompareTo(Activite o)
        {
            return _name.CompareTo(o.Nom);
        }

        #endregion
    }
}
