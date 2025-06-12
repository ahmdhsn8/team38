#define _USE_MATH_DEFINES
#include<iostream>
#include<string>
#include<cmath>
#include <vector>

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
            this-> density=density;
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
    double r, l,m;
    circle () {}
    double Area ()
    {
        return r*r*PI ;
    }
    double Inertia()
        {
            return (PI * pow(r, 4)) / 4.0;
        }
    double circMaxStress(double m, double r, double i)
        {
            return (m*r)/(circle::Inertia());
        }
    double circMass(double p, double r, double l)
        {
            return p*M_PI*pow(r,2)*l;
        }
    
};
class rectangle
{
    public:
    double h, b,l; // height hwa h w width hwa b
    double Area ()
    {
        return h*b ;
    }
    double Inertia()  {
        return (b * pow(h, 3)) / 12.0;
    }
    double recMaxStress (double m, double h, double i)
    {
        return (m*h)/(2*rectangle::Inertia());
    }
            //Rectangle
    double recMass(double p, double b, double h, double l)
    {
        return p*b*h*l;
    }
};

    //Bending Moment
    //Calculate Bending Moment
        // The moment at the base of the link includes:
        // Weight force (𝑊 = 𝑚𝑔).
        // Inertial force (𝐹 = 𝑚 𝑎).
        // Equation:
        // 𝑀 = 𝑚𝑙∗𝑔∗(𝐿 / 2)+(𝑚𝑝∗𝑔∗ 𝐿)+(𝑚𝑙∗(𝐿2)2∗𝛼𝑚𝑎𝑥+𝑚𝑝∗𝐿2∗𝛼𝑚𝑎𝑥)
        // Where:
        // 𝑔 = 9.81 𝑚/𝑠²
        // 𝑚𝑙.𝑔.𝐿/2: Weight of link acting at its center of mass.
        // (𝑚𝑝∗𝑔∗ 𝐿): Weight of payload at end-effector.
        // (𝑚𝑙∗(𝐿2)2∗𝛼𝑚𝑎𝑥+𝑚𝑝∗𝐿2∗𝛼𝑚𝑎𝑥): Inertial force.
    double bendingMoment(double ml, double l, double mP, double alphaMaX)
    {
        return ml*9.81*l*0.5 + mP*9.81*l + ml *pow((0.5*l),2) *alphaMaX + mP*pow(l,2)*alphaMaX;
    }


int check_flag =1; // flag for sigma checking

int sigma_check (double sigma_calc , double sigma_yield) // This compares between sigma calculated & sigma yield
{
    if( sigma_calc < sigma_yield)         // makes check_flag =1 is sigma calc is smaller
    {
        check_flag =1;
        return 1;
    }
    else if( sigma_calc >= sigma_yield)     // makes check_flag =0 is sigma yield is smaller
    {
        check_flag =0;
        return 0 ;
    }
}

void dim_opti_rec (double& b , double& h)   // iteration function
{
    if (check_flag ==1 )                    // increasing dimensions iterations 
    {
        b +=  0.01 * b;
        h +=  0.01 *h;
    }
    else if (check_flag ==0)                   // decreasing dimension iterations 
    {
        b -= 0.01 * b;
        h -= 0.01 * h;
    }
}


void dim_opti_cir (double& radius)   // iteration function
{
    if (check_flag ==1 )                    // increasing dimensions iterations 
    {
        radius +=  0.01 * radius;
        
    }
    else if (check_flag ==0)                   // decreasing dimension iterations 
    {
        radius -= 0.01 * radius;
    }
}
    
int main()
{
    vector <Material> materials = {
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
    int choice;
    cout << "\nEnter the number of the material you want to select: ";
    cin >> choice;
    while (choice < 1 || choice > (materials.size()+1)) {
        cout << "Invalid choice. Enter a valid number: ";
        cin >> choice;
    }
    if (choice==(materials.size()+1) )
    {
        string newmaterial ;
        double newyield_strength;
        double newdensity;
        cout<<"\nNew Material Name : ";
        cin >> newmaterial ;
        cout<<"Yield Strength in MPa : " ;
        cin >> newyield_strength ;
        cout<<"Density in kg/m3: ";
         cin >> newdensity ;
         Material custom (newmaterial, newyield_strength, newdensity);
         materials.push_back(custom);

    }
    Material selected = materials[choice - 1];
    selected.display_material_properties();
    double alphaMaX ;
    double mP ; // payload variable
    double I ;
    rectangle T1 ;
    circle C1 ;
    string x ;
    cout << "\nWhat is the cross section type (circle or rectangle): ";
    cin >> x ;
    if (x== "circle" ||x== "Circle") // mesh gmani el mokarna bs it worked
    {
        cout << "\n Circle radius = ";
        cin >> C1.r ;
        double radius = C1.r;
        cout << "\n Member length = ";
        cin >> C1.l ;
        double length = C1.l;
        I = C1.Inertia() ;

    }
    else if (x== "Rectangle" ||x== "rectangle")
    {
        cout << "\n Rectangle hieght = ";
        cin >> T1.h ;
        double height = T1.h;
        cout << "\n Rectangle Width = ";
        cin >> T1.b ;
        double width = T1.b;
        cout << "\n Member length = ";
        cin >> T1.l ;
        double length = T1.l;
        I = T1.Inertia() ;
    }
    cout << "What is the pay load : " ;
    cin >> mP ;
    cout << "\n What is the Maximum angular accelaration : " ;
    cin >> alphaMaX ;
}