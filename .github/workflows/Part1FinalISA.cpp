#define _USE_MATH_DEFINES
#include<iostream>
#include<string>
#include<cmath>
#include <vector>

using namespace std;

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


const double g = 9.81; //m/s2
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
            cout<<"Density: "<<density<<" g/cm³\n\n";
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
    long double Area ()
    {
        return r*r*M_PI ;
    }
    long double Inertia()
        {
            return (M_PI * pow(r, 4)) / 4.0;
        } 
    long double circMaxStress()
        {
            return (bendingMoment()*1000*r)/Inertia(); //gives MPa
        }
    long double circMass()
        {
            return p*M_PI*pow(r,2)*l*pow(10,-6); // gives kilogram
        }
    long double bendingMoment()
    {
        return (circMass()*9.81*l*0.5*pow(10,-3) + mP*9.81*l*pow(10,-3) + (circMass() *pow((0.5*l*pow(10,-3)),2) *alphaMax + mP*pow(l*pow(10,-3),2)*alphaMax));
    }    // gives N.m
    
};
class rectangle
{
    public:
    double h, b,l,mP,alphaMax,p,yield; // height hwa h w width hwa b
    long double Area ()
    {
        return h*b ;
    }
    long double Inertia()  {
        return (b * pow(h, 3)) / 12.0;
    }
    long double recMaxStress ()
    {
        return (bendingMoment()*1000*h)/(2*Inertia());
    }
            //Rectangle
    long double recMass()
    {
        return p*b*h*l*pow(10,-6);
    }
    long double bendingMoment()
    {
        return recMass()*9.81*l*pow(10,-3)*0.5 + mP*9.81*l*pow(10,-3) + (recMass() *pow((0.5*l*pow(10,-3)),2) *alphaMax + mP*pow(l*pow(10,-3),2)*alphaMax);
    }
};
void flow_func_circ (string shapeType, circle& C)  //hanwsal l7d as8r aw akbar mn sigma yield b 0.1
{
    double sigma_calc= C.circMaxStress();
    //cout<<sigma_calc<<"\n";
    double sigma_yield= C.yield;
    int max_iter=100000000000000;
    int iter=0;
    if (shapeType=="Circle" && sigma_calc < sigma_yield)
    {
        while (sigma_calc < (sigma_yield - 0.5)  ) //&& iter < max_iter
        {
            C.r -=0.01 * C.r;
            sigma_calc = C.circMaxStress();
            //cout << C.r<<"\n";
            //cout<< sigma_calc<<"\n";
            iter++;
            
        }
        
    }
    else if (shapeType=="Circle" && sigma_calc > sigma_yield )
    {
        while (sigma_calc > (sigma_yield + 0.5)  ) //&& iter < max_iter
        {
            C.r +=0.1 * C.r;
            sigma_calc = C.circMaxStress();
            //cout << C.r<<"\n";
            //cout<< sigma_calc<<"\n";
            iter++;
        }
            //cout << C.r<<"\n";
            //cout<< sigma_calc<<"\n";
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
    int max_iter=100000000000000;
    int iter=0;
    if (sigma_calc < sigma_yield)
    {
        while (sigma_calc < (sigma_yield - 2) && iter < max_iter)
        {
            T.b -=0.01 * T.b;
            T.h -=0.01 * T.h;
            sigma_calc = T.recMaxStress();
            // cout << T.b<<"\n";
            // cout << T.h <<"\n";
            // cout<< sigma_calc<<"\n";
            iter++;
        }
        if (T.b > T.h)
        {
           while (sigma_calc < (sigma_yield - 2) && iter < max_iter)
            {
                T.b -=0.0001 * T.b;
               // T.h -=0.0001 * T.h;
                sigma_calc = T.recMaxStress();
                //cout << T.b<<"\n";
                //cout<< sigma_calc<<"\n";
                iter++;
            }
        }
        else
        {
            while (sigma_calc < (sigma_yield -2) && iter < max_iter)
            {
                T.h -=0.0001 * T.h;
                sigma_calc = T.recMaxStress();
                //cout << T.b<<"\n";
                //cout<< sigma_calc<<"\n";
                iter++;
            }
        }
    
    }
    else if (sigma_calc > sigma_yield )
    {
        while (sigma_calc > (sigma_yield + 2)  && iter < max_iter )
        {
            T.b +=0.01 * T.b;
            T.h +=0.01 * T.h;
            sigma_calc = T.recMaxStress();
            //cout << T.b<<"\n";
            //cout << T.h <<"\n";
            //cout<< sigma_calc<<"\n";
            iter++;
        }
        if (T.b < T.h)
        {
             while (sigma_calc > (sigma_yield + 2) && iter < max_iter)
            {
            T.b +=0.0001 * T.b;
            //T.h +=0.0001 * T.h;
            sigma_calc = T.recMaxStress();
            //cout << T.b<<"\n";
            //cout<< sigma_calc<<"\n";
            iter++;
            }
        }
        else 
        {
            while (sigma_calc > (sigma_yield + 2) && iter < max_iter)
            {
            T.h +=0.0001 * T.h;
            //T.h +=0.0001 * T.h;
            sigma_calc = T.recMaxStress();
            //cout << T.b<<"\n";
            //cout<< sigma_calc<<"\n";
            iter++;
            }
        }

       
    }
    cout<< "iterations = "<< iter;
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
    int choice;
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
        double newdensity = ValidDouble("Density in g/cm3: ");
         Material custom (newmaterial, newyield_strength, newdensity);
         materials.push_back(custom);

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
        cout<<"Density in g/cm3: ";
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
    while (true)
    {
        cout << "\nwhat is the cross section type (circle or rectangle): ";
        cin >> x ;
    if (x== "circle" ||x== "Circle" ||x== "c") // mesh gmani el mokarna bs it worked
    {
        C1.r = ValidDouble("\n circle radius in mm = ");
        C1.l = ValidDouble("\n Member length in mm = ");
        C1.p = selected.getDensity();
        C1.yield =selected.getYieldStrength();
        cout << "What is the pay load in kilogram : " ;
        cin >> C1.mP ;
        cout << "\n What is the Maximum angular accelaration rad/s2 : " ;
        cin >> C1.alphaMax ;
        flow_func_circ("Circle",C1);
        cout<< "Member length :"<<C1.l;
        cout << "\n--- Optimization Complete ---\n";
        cout << "Final Optimized Radius: " << C1.r << " mm\n";
        cout << "Final Stress: " << C1.circMaxStress() << " MPa\n";
        cout << "Bending Moment: " << C1.bendingMoment() << " Nm\n";
        cout << "Mass: " << C1.circMass() << " kg\n"; 
        break;
    }
    else if (x== "Rectangle" ||x== "rectangle"||x=="r")
    {
        T1.h = ValidDouble("\n rectangle height in mm = ");
        T1.b = ValidDouble("\n rectangle width in mm = ");
        T1.l = ValidDouble("\n Member length in mm= ");
        T1.p = selected.getDensity();
        T1.yield =selected.getYieldStrength();
        cout << "What is the pay load in kilogram : " ;
        cin >> T1.mP ;
        cout << "\n What is the Maximum angular accelaration rad/s2 : " ;
        cin >> T1.alphaMax ;
        flow_func_rec(T1);
        cout << "\n--- Optimization Complete ---\n";
        cout << "Final Optimized height: " << T1.h << " mm\n";
        cout << "Final Optimized width: " << T1.b << " mm\n";
        cout << "Final Stress: " << T1.recMaxStress() << " MPa\n";
        cout << "Bending Moment: " << T1.bendingMoment() << " Nm\n";
        cout << "Mass: " << T1.recMass() << " g\n"; 
        break;
    }
    else 
    {
        cin.clear();
        cout << "Invalid input. Please enter 'circle' or 'rectangle'.\n";
        continue; 
    }
    }
}