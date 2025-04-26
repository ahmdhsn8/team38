#define _USE_MATH_DEFINES
#include<iostream>
#include<string>
#include<cmath>
#include <vector>
#include <limits>
using namespace std;
double ValidDouble(string prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (!cin.fail()&& value>0 ) break;
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
double chosen_yield =0;

class Motor
{
public:
    double torque;
    double speed;
    double mass;
    double diameter;
    double width;
    string name;
    //constructor input l motors
    Motor(string name, double torque, double speed, double mass, double diameter, double width)
    {
        this-> name = name;
        this-> torque = torque;
        this -> speed = speed;
        this -> mass = mass;
        this -> diameter = diameter;
        this -> width = width;

    }

    // fn output l properties
    void display_motor_properties()
    {
        cout<<"Model: "<<name<<"\n";
        cout<<"Tourque: "<<torque<<" mNm"<<"\n";
        cout<<"Speed: "<<speed<<" rpm"<<"\n";
        cout<<"Mass: "<<mass<<" g"<<"\n";
        cout<<"Diameter: "<<diameter<<" mm"<<"\n";
        cout<<"Width: "<<width<<" mm"<<"\n";
    }
    string getName(){
        return this->name;
    }

};

class Gearbox
{
public:
    string name;
    double redRatio;
    double mass;
    double diameter;
    double width;
    double eff;
    Gearbox() {}
    Gearbox(string name, double redRatio, double mass, double diameter, double width, double eff)
    {
        this -> name = name;
        this -> redRatio = redRatio;
        this -> mass = mass;
        this -> diameter = diameter;
        this -> width = width;
        this -> eff = eff;
    }

    void display_gear_properties()
    {
        cout << "Model: "<<name<<"\n";
        cout << "Reduction Ratio: "<<redRatio<<"\n";
        cout << "Mass: "<<mass<<" Kg\n";
        cout << "Diameter: "<<diameter<<" mm\n";
        cout << "Width: "<< width<< " mm\n";
        cout << "Efficency: "<< eff<< " %\n";
    }

};

class Pairs
{
public:
    Motor* M_REF;
    Gearbox* G_REF;
    float Tout;
    float Wout;
    float cost;
    float Mtotal;
    float Dtotal;
    float Wtotal;
    Pairs(){}
    Pairs(Motor &MID, Gearbox &GID)
    {
        this -> M_REF = &MID;
        this -> G_REF = &GID;
    }

};

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
    double r, l,m,p,I,mP,alphaMax,yield,stepRatio;
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
    double h, b,l,mP,alphaMax,p,yield,stepRatio; // height hwa h w width hwa b
    long double Area ()
    {
        return h*b ;
    }
    long double Inertia()
    {
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


//Torque calculation
//Required Torque Calculation
double torqueRec(double ml, double l, double mp, double sig)
{
    return ml*9.81*l*0.5+mp*9.81*l+ml*pow((l*0.5),2)*sig +mp*pow(l,2)*sig;
}
//Motor gear box output tourque calculation
double torqueMotorGear(double tMotor, double ratio, double efficency)
{
    return tMotor*ratio*efficency;
}
//Motor-Gearbox output speed calculation
double speedMotorGear(double w, double ratio )
{
    return w*ratio;
}
//Compute Cost fn
double cost(double m, double d, double w)
{
    return m+ d/100 + w/100;
}

void flow_func_circ (string shapeType, circle& C)  //hanwsal l7d as8r aw akbar mn sigma yield b 0.1
{
    double sigma_calc= C.circMaxStress();
    //cout<<sigma_calc<<"\n";
    double sigma_yield= C.yield;
    long long int max_iter=pow(10,10);
    int iter=0;
    double stepRatio =1;
    if (shapeType=="Circle" && sigma_calc < sigma_yield)
    {
        while (sigma_calc < (sigma_yield - 0.5)  ) //&& iter < max_iter
        {
            C.r -= C.stepRatio /100 * C.r;
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
            C.r += C.stepRatio /100 * C.r;
            sigma_calc = C.circMaxStress();
            //cout << C.r<<"\n";
            //cout<< sigma_calc<<"\n";
            iter++;
        }
            //cout << C.r<<"\n";
            //cout<< sigma_calc<<"\n";
    }
    cout <<"\n number of iteration = "<<iter<<"\n";
    if (iter >= max_iter)
    {
        cout << "\n Optimization failed: reached max iterations.\n";
    }
}
void flow_func_rec (rectangle& T)  //hanwsal l7d as8r aw akbar mn sigma yield b 0.1
{
    double sigma_calc= T.recMaxStress();
    double sigma_yield= T.yield;
    long long int max_iter= pow(10,10);
    int iter=0;
    double stepRatio =1;
    if (sigma_calc < sigma_yield)
    {
        while (sigma_calc < (sigma_yield - 2) && iter < max_iter)
        {
            T.b -= T.stepRatio /100 * T.b;
            T.h -= T.stepRatio /100 * T.h;
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
            T.b += T.stepRatio /100 * T.b;
            T.h += T.stepRatio /100 * T.h;
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
    cout <<"\n number of iteration = "<<iter<<"\n";
    if (iter >= max_iter)
    {
        cout << "\n Optimization failed: reached max iterations.\n";
    }
}
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

vector <Motor> motors =
{
    Motor("Motor 1", 0.322, 6650, 225, 50, 22),
    Motor("Motor 2", 1.710, 3410, 741, 85, 33),
    Motor("Motor 3", 0.430, 4330, 270, 50, 27),
    Motor("Motor 5", 0.688, 4570, 377, 65, 25),
    Motor("Motor 6", 1.130, 2590, 524, 65, 33),
    Motor("Motor 7", 0.186, 6290, 170, 38, 25),
    Motor("Motor 8", 0.097, 12400, 125, 38, 19)
};

vector <Gearbox> Gearboxes =
{
    Gearbox("GB 12 worm gear", 30/1, 1.5, 12, 62, 65),
    Gearbox("Planetary gearhead GP 42 A", 1296/1, .56,42,155.6,64),
    Gearbox("Planetary Gearhead GP 16 A Ø16 mm, 0.1 - 0.3 Nm, Metal Version, Sleeve Bearing", 4.4/1, .02, 16, 52.3, .9)
};

void Type_func (double alphaMaX ,double mP ,long double I ,rectangle T1 ,circle C1 ,string x )
{

}


int main()
{

    int choice;
    cout << "Choose a material:\n";
    for (int i = 0; i < materials.size(); i++)
    {
        cout << i + 1 << "- " << materials[i].getName() << "\n";
    }
    cout << (materials.size()+1)<< "- new material   " ;
    choice = ValidInt( 1, materials.size() + 1);
    if (choice==(materials.size()+1) )
    {
        string newmaterial = ValidString("New Material Name: ");
        double newyield_strength = ValidDouble("Yield Strength in MPa: ");
        double newdensity = ValidDouble("Density in g/cm^3: ");
         Material custom (newmaterial, newyield_strength, newdensity);
         materials.push_back(custom);

    }
    Material selected = materials[choice - 1];
    selected.display_material_properties();
    rectangle T1 ;
    circle C1 ;
    string x ;
    while (true)
    {
        cout << "\nwhat is the cross section type (circle or rectangle): ";
        cin >> x ;
    if (x== "circle" ||x== "Circle" ||x== "c") // mesh gmani el mokarna bs it worked
    {
        C1.r = ValidDouble("\n circle radius (in mm) = ");
        C1.l = ValidDouble("\n Member length (in mm) = ");
        C1.p = selected.getDensity();
        C1.yield =selected.getYieldStrength();
        C1.mP= ValidDouble ( "\n What is the pay load (in kilogram) : " ); 
        C1.alphaMax = ValidDouble ( "\n What is the Maximum angular accelaration (rad/s^2) : ") ;
        C1.stepRatio = ValidDouble ("\n What is step ratio needed in % (default value = 1%) : ");
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
        T1.h = ValidDouble("\n rectangle height (in mm) = ");
        T1.b = ValidDouble("\n rectangle width (in mm) = ");
        T1.l = ValidDouble("\n Member length (in mm) = ");
        T1.p = selected.getDensity();
        T1.yield =selected.getYieldStrength();
        T1.mP= ValidDouble ( "\n What is the pay load (in kilogram) : " ); 
        T1.alphaMax = ValidDouble ( "\n What is the Maximum angular accelaration (rad/s^2) : ") ;
        T1.stepRatio = ValidDouble ("\n What is step ratio needed in % (default value = 1%) : ");
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
    double Treq = torqueRec(C1.circMass(), C1.l/1000, C1.mP, C1.alphaMax);
    double Wreq = 1000;
    vector <Pairs> PairsV;

    for (int i = 0; i < motors.size(); i++)
    {
        for (int j = 0; j < Gearboxes.size(); j++)
        {
            double Tout = torqueMotorGear(motors[i].torque, Gearboxes[j].redRatio, Gearboxes[j].eff);
            double Wout = speedMotorGear(motors[i].speed, Gearboxes[j].redRatio);
            if(Tout >= Treq)
            {
                Pairs tempPair;
                tempPair.M_REF = &motors[i];
                tempPair.G_REF = &Gearboxes[j];
                PairsV.push_back(tempPair);
            }
        }
    }
    for (int j = 0; j < PairsV.size(); j++)
        {
            cout << "Pair No {" << j << "}: " << PairsV[j].M_REF->getName() << " with " << PairsV[j].G_REF->name << endl;
        }
}
