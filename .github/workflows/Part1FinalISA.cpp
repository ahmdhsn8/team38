#define _USE_MATH_DEFINES
#include<iostream>
#include<string>
#include<cmath>
#include <vector>

using namespace std;
//double bendingMoment(double ml, double l, double mP, double alphaMaX);
const float PI= 3.14159;
const double g = 9.81;
double chosen_yield=0;
double MaxStress=0;
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
            chosen_yield =yield_strength;
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
    double r, l,m,p,I,mP,alphaMax,yield;
    circle () {}
    double Area ()
    {
        return r*r*M_PI ;
    }
    double Inertia()
        {
            //double I=(M_PI * pow(r, 4)) / 4.0;
            return (M_PI * pow(r, 4)) / 4.0;
        }
    long double circMaxStress()
        {
            MaxStress =(bendingMoment()*r)/Inertia();
            return (bendingMoment()*r)/Inertia();
        }
    double circMass()
        {
            return p*M_PI*pow(r,2)*l;
        }
    long double bendingMoment()
    {
        return circMass()*9.81*l*0.5 + mP*9.81*l + circMass() *pow((0.5*l),2) *alphaMax + mP*pow(l,2)*alphaMax;
    }
    
};
class rectangle
{
    public:
    double h, b,l,mP,alphaMax,p,yield; // height hwa h w width hwa b
    double Area ()
    {
        return h*b ;
    }
    double Inertia()  {
        return (b * pow(h, 3)) / 12.0;
    }
    double recMaxStress ()
    {
        MaxStress =(bendingMoment()*h)/(2*Inertia());
        return (bendingMoment()*h)/(2*Inertia());
    }
            //Rectangle
    double recMass()
    {
        return p*b*h*l;
    }
    double bendingMoment()
    {
        return recMass()*9.81*l*0.5 + mP*9.81*l + recMass() *pow((0.5*l),2) *alphaMax + mP*pow(l,2)*alphaMax;
    }
};
void flow_func_circ (string shapeType, circle& C)  //hanwsal l7d as8r aw akbar mn sigma yield b 0.1
{
    double sigma_calc= C.circMaxStress();
    double sigma_yield= C.yield;
    int max_iter=10000;
    int iter=0;
    if (shapeType=="Circle" && sigma_calc < sigma_yield)
    {
        while (sigma_calc < (sigma_yield - 0.1) && iter < max_iter)
        {
            C.r +=0.1 * C.r;
            sigma_calc = C.circMaxStress();
            iter++;
        }
        
    }
    else if (shapeType=="Circle" && sigma_calc > sigma_yield )
    {
        while (sigma_calc > sigma_yield + 0.1  && iter < max_iter )
        {
            C.r -=0.1 * C.r;
            sigma_calc = C.circMaxStress();
            iter++;
        }
    }
    if (iter >= max_iter) 
    {
        cout << "\n Optimization failed: reached max iterations.\n";
    }
}
void flow_func_rec (rectangle& T)  //hanwsal l7d as8r aw akbar mn sigma yield b 0.1
{
    double sigma_calc= T.recMaxStress();
    double sigma_yield= T.yield;
    int max_iter=1000;
    int iter=0;
    if (sigma_calc < sigma_yield)
    {
        while (sigma_calc < (sigma_yield - 0.1) && iter < max_iter)
        {
            T.b +=0.1 * T.b;
            T.h +=0.1 * T.h;
            sigma_calc = T.recMaxStress();
            iter++;
        }
        
    }
    else if (sigma_calc > sigma_yield )
    {
        while (sigma_calc > sigma_yield + 0.1  && iter < max_iter )
        {
            T.b -=0.1 * T.b;
            T.h -=0.1 * T.h;
            sigma_calc = T.recMaxStress();
            iter++;
        }
    }
    if (iter >= max_iter) 
    {
        cout << "\n Optimization failed: reached max iterations.\n";
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
    string newmaterial ;
    double newyield_strength;
    double newdensity;
    if (choice==(materials.size()+1) )
    {
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
    long double I ;
    rectangle T1 ;
    circle C1 ;
    string x ;
    cout << "\nWhat is the cross section type (circle or rectangle): ";
    cin >> x ;
    if (x== "circle" ||x== "Circle" ||x== "c") // mesh gmani el mokarna bs it worked
    {
        cout << "\n Circle radius = ";
        cin >> C1.r ;
        cout << "\n Member length = ";
        cin >> C1.l ;
        C1.p = selected.getDensity();
        C1.yield =selected.getYieldStrength();
        I = C1.Inertia() ;
        cout << "What is the pay load : " ;
        cin >> C1.mP ;
        cout << "\n What is the Maximum angular accelaration : " ;
        cin >> C1.alphaMax ;
        flow_func_circ("Circle",C1);

        
        cout << "\n--- Optimization Complete ---\n";
        cout << "Final Optimized Radius: " << C1.r << " m\n";
        cout << "Final Stress: " << C1.circMaxStress() << " MPa\n";
        cout << "Bending Moment: " << C1.bendingMoment() << " Nm\n";
        cout << "Mass: " << C1.circMass() << " kg\n"; 
    }
    else if (x== "Rectangle" ||x== "rectangle"||x=="r")
    {
        cout << "\n Rectangle hieght = ";
        cin >> T1.h ;
        cout << "\n Rectangle Width = ";
        cin >> T1.b ;
        cout << "\n Member length = ";
        cin >> T1.l ;
        C1.p = selected.getDensity();
        C1.yield =selected.getYieldStrength();
        cout << "What is the pay load : " ;
        cin >> T1.mP ;
        cout << "\n What is the Maximum angular accelaration : " ;
        cin >> T1.alphaMax ;
        flow_func_rec(T1);

        cout << "\n--- Optimization Complete ---\n";
        cout << "Final Optimized height: " << T1.h << " m\n";
        cout << "Final Optimized width: " << T1.b << " m\n";
        cout << "Final Stress: " << T1.recMaxStress() << " MPa\n";
        cout << "Bending Moment: " << T1.bendingMoment() << " Nm\n";
        cout << "Mass: " << T1.recMass() << " kg\n"; 
    }
}