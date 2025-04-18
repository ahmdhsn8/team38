#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
const float PI= 3.14159; 
const double g = 9.81; 
class Material{
    protected:
        string name;
        double yield_strength;
        double density;
    
    public:
        //function constructor bta3 input l material
        Material(string name, double yield_strength, double density){
            this-> name = name;
            this-> yield_strength= yield_strength;
            this->density=density;
        }
        //fn output l properties 
        void display_material_properties(){
            cout<<"Material: "<<name<<"\n";
            cout<<"Yield Strength: "<<yield_strength<<" Mpa\n";
            cout<<"Density: "<<density<<" kg/m³\n\n";
        }
        string getName() {
            return name;
        }

        double getDensity() {
             return density;
        }

        double getYieldStrength() {
            return yield_strength;
        }

    };
class circle 
{
    public:
    double r, l;
    circle () {}  
    double Area ()
    {
        return r*r*PI ; 
        
    }
    double Inertia() 
        {
            return (PI * pow(r, 4)) / 4.0;
        }
};
class rectangle  
{
    public:
    double h, b,l; // height hwa h w width hwa b 
    double Area ( )
    {
        return h*b ; 
    }
    double Inertia()  {
        return (b * pow(h, 3)) / 12.0;
    }
};

int main()
{
    vector<Material> materials = {
        Material("Steel", 247, 7.58),
        Material("Cast Iron", 130, 7.3),
        Material("Copper Nickel", 130, 8.94),
        Material("Brass", 200, 8.73),
        Material("Aluminium", 241, 2.7),
        Material("Acrylic", 72, 1.16),
        Material("Copper", 70, 8.92),
        Material("Stainless Steel", 275, 7.86),
        Material("Tungsten", 941, 19.75)
    };

    cout << "Choose a material:\n";
    for (int i = 0; i < materials.size(); i++) {
        cout << i + 1 << "- " << materials[i].getName() << "\n";
    }
    cout << "11- new material   " ; 
    int choice;
    cout << "\nEnter the number of the material you want to select: ";
    cin >> choice;

    while (choice < 1 || choice > 11) {
        cout << "Invalid choice. Enter a valid number: ";
        cin >> choice;
    }
    if (choice==11 )
    {
        string newmaterial ;
        double newyield_strength;
        double newdensity;
        cout<<"New Material Name : ";
        cin >> newmaterial ;
        cout<<"\nYield Strength in MPa : " ; 
        cin >> newyield_strength ; 
        cout<<"Density in kg/m3: ";
         cin >> newdensity ; 
         Material custom (newmaterial, newyield_strength, newdensity);
         materials.push_back(custom);
         
    }
    Material selected = materials[choice - 1];
    selected.display_material_properties();
    double Maxalpha ; 
    double Mp ; // payload variable 
    double I ; 
    rectangle T1 ;
    circle C1 ;
    string x ; 
    cout << "\n what is the cross section type (circle or rectangle): ";
    cin >> x ; 
    if (x== "circle" ||x== "Circle") // mesh gmani el mokarna bs it worked 
    {
        cout << "\n circle radius = "; 
        cin >> C1.r ; 
        double radius = C1.r;
        cout << "\n Member length = "; 
        cin >> C1.l ;
        double length = C1.l;
        I = C1.Inertia() ; 

    }
    else if (x== "Rectangle" ||x== "rectangle")
    {
        cout << "\n rectangle hieght = "; 
        cin >> T1.h ; 
        double height = T1.h;
        cout << "\n rectangle Width = "; 
        cin >> T1.b ; 
        double width = T1.b;
        cout << "\n Member length = "; 
        cin >> T1.l ;
        double length = T1.l;
        I = T1.Inertia() ; 
    } 
    cout << "what is the pay load : " ; 
    cin >> Mp ; 
    cout << "\n what is the Maximum angular accelaration : " ; 
    cin >> Maxalpha ; 
}