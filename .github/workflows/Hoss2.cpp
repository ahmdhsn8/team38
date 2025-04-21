#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;
const float PI= 3.14159;
const double g = 9.81;
double ValidDouble(string prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (!cin.fail()) break;
        cin.clear(); // clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << "Invalid input. Please enter a number.\n";
        continue; 
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove leftover newline
    return value;
}

int ValidInt( int min, int max) {
    int value;
    while (true) {
        cout << "Enter the number of the material you want to select: ";
        cin >> value;
        if (!cin.fail() && value >= min && value <= max) break;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
        continue;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove leftover newline
    return value;
}

string ValidString(string y) {
    string value;
    cout << y;
    getline(cin >> ws, value); 
    return value;
}

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
    double height,radius , length , width, I , Mp, Maxalpha , A;
    int choice;
    rectangle T1 ;
    circle C1 ;
    string x ;
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
    cout << (materials.size()+1)<< "- new material   " ;
    choice = ValidInt( 1, materials.size() + 1);
    if (choice==(materials.size()+1) )
    {
        string newmaterial = ValidString("New Material Name: ");
        double newyield_strength = ValidDouble("Yield Strength in MPa: ");
        double newdensity = ValidDouble("Density in kg/m3: ");
         Material custom (newmaterial, newyield_strength, newdensity);
         materials.push_back(custom);

    }
    Material selected = materials[choice - 1];
    selected.display_material_properties();
    while (true)
    {
        cout << "\nwhat is the cross section type (circle or rectangle): ";
    cin >> x ;
    if (x== "circle" ||x== "Circle") 
    {
        C1.r = ValidDouble("\n circle radius = ");
         radius = C1.r;
        C1.l = ValidDouble("\n Member length = ");
         length = C1.l;
        I = C1.Inertia() ;
        A= C1.Area() ;
        break;
    }
    else if (x== "Rectangle" ||x== "rectangle")
    {
        T1.h = ValidDouble("\n rectangle hieght = ");
        height = T1.h;
        T1.b = ValidDouble("\n rectangle width = ");
        width = T1.b;
        T1.l = ValidDouble("\n Member length = ");
        length = T1.l;
        I = T1.Inertia() ;
        A= T1.Area() ;
        break;
    }
    else 
    {
        cin.clear();
        cout << "Invalid input. Please enter 'circle' or 'rectangle'.\n";
        continue; 
    }

    }   
    Mp = ValidDouble("\n what is the payload : " );
    Maxalpha = ValidDouble("\n what is the maximum angular acceleration : " );
    cout << "\n The maximum angular acceleration is : " << Maxalpha << " rad/s²\n";
    cout << "\n The payload is : " << Mp << " N\n";
    cout << "\n The moment of inertia is : " << I << " kg.m²\n";
    cout << "\n The length of the member is : " << length << " m\n";
    cout << "\n The radius of the member is : " << radius << " m\n";
    cout << "\n The height of the member is : " << height << " m\n";
    cout << "\n The width of the member is : " << width << " m\n";
    cout << "\n The density of the material is : " << selected.getDensity() << " kg/m³\n";
    cout << "\n The yield strength of the material is : " << selected.getYieldStrength() << " Mpa\n";
    cout << "\n The area of the member is : " <<A << " m²\n";
}