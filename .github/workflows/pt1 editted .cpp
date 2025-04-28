#define _USE_MATH_DEFINES
#include<iostream>
#include<string>
#include<cmath>
#include <vector>
#include <limits>
using namespace std;
const double g = 9.81; //m/s2
double chosen_yield =0;

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
        void display_material_properties() const{
            cout<<"Material: "<<name;
            cout<<"\nYield Strength: "<<yield_strength<<" Mpa\n";
            cout<<"Density: "<<density<<" g/cm³\n";
            chosen_yield =yield_strength;
        }
        string getName() const {
            return name;
        }
        double getDensity()const {
             return density;
        }
        double getYieldStrength() const {
            return yield_strength;
        }
    };
    int chooseMaterial() {
        cout << "Choose a material:\n";
        for (int i = 0; i < materials.size(); i++)
            cout << i + 1 << "- " << materials[i].getName() << "\n";
        cout << materials.size() + 1 << "- New Material\n";

        int choice = ValidInt(1, materials.size() + 1);

        if (choice == materials.size() + 1) {
            string newName = ValidString("New Material Name: ");
            double newYield = ValidDouble("Yield Strength (MPa): ");
            double newDensity = ValidDouble("Density (g/cm^3): ");
            materials.emplace_back(newName, newYield, newDensity);
            choice = materials.size();
        }
        materials[choice - 1].display_material_properties();
        return choice - 1;
    }
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

void handleC(const Material& selected, circle& C1) {
    C1.r = ValidDouble("\nCircle radius (mm): ");
    C1.l = ValidDouble("Member length (mm): ");
    C1.p = selected.getDensity();
    C1.yield = selected.getYieldStrength();
    C1.mP = ValidDouble("Payload (kg): ");
    C1.alphaMax = ValidDouble("Max angular acceleration (rad/s²): ");
    C1.stepRatio = ValidDouble("Step ratio (%) [default 1%]: ");
    flow_func_circ("Circle", C1);

    cout << "\n--- Optimization Complete ---\n"
         << "Final Radius: " << C1.r << " mm\n"
         << "Final Stress: " << C1.circMaxStress() << " MPa\n"
         << "Bending Moment: " << C1.bendingMoment() << " Nm\n"
         << "Mass: " << C1.circMass() << " kg\n";
}

void handleR(const Material& selected, rectangle& T1) {
    T1.h = ValidDouble("\nRectangle height (mm): ");
    T1.b = ValidDouble("Rectangle width (mm): ");
    T1.l = ValidDouble("Member length (mm): ");
    T1.p = selected.getDensity();
    T1.yield = selected.getYieldStrength();
    T1.mP = ValidDouble("Payload (kg): ");
    T1.alphaMax = ValidDouble("Max angular acceleration (rad/s²): ");
    T1.stepRatio = ValidDouble("Step ratio (%) [default 1%]: ");
    flow_func_rec(T1);

    cout << "\n--- Optimization Complete ---\n"
         << "Final Height: " << T1.h << " mm\n"
         << "Final Width: " << T1.b << " mm\n"
         << "Final Stress: " << T1.recMaxStress() << " MPa\n"
         << "Bending Moment: " << T1.bendingMoment() << " Nm\n"
         << "Mass: " << T1.recMass() << " kg\n";
}
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
// List of all motors
vector<Motor> motors;
// the user keeps adding motors as much as he wants
void adding_motors()//ha7tag a7ot adding_motors(); fel main ... matensash //
{
    while (true) {
        cout << "\nAdding a new Motor:\n";

        string name;
        cout << "Enter Motor Name: ";
        getline(cin, name);

        double torque;
        cout << "Enter Motor Torque (mNm): ";
        cin >> torque;

        double speed;
        cout << "Enter Motor Speed (rpm): ";
        cin >> speed;

        double mass;
        cout << "Enter Motor Mass (kg): ";
        cin >> mass;

        double diameter;
        cout << "Enter Motor Diameter (mm): ";
        cin >> diameter;

        double width;
        cout << "Enter Motor Width (mm): ";
        cin >> width;


        Motor newMotor(name, torque, speed, mass, diameter, width);

        // Push it into the vector
        motors.push_back(newMotor);

        // Ask if they want to add another
        cout << "\nDo you want to add another motor? (y/n): ";
        char choice;
        cin >> choice;

        if (choice != 'y') {
            break;
        }
    }
}


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
// List of all gearboxes
vector<Gearbox> gearboxes;
// the user keeps adding gearboxes as much as he wants
void adding_gearboxes()//ha7tag a7ot adding_gearboxes(); fel main ... matensash //
{

    while (true) {
        cout << "\nAdding a new Gearbox:\n";

        string name;
        cout << "Enter Gearbox Name: ";
        cin >> ws;
        getline(cin, name);

        double reductionRatio;
        cout << "Enter Gearbox Reduction Ratio: ";
        cin >> reductionRatio;

        double efficiency;
        cout << "Enter Gearbox Efficiency (e.g., 0.85 for 85%): ";
        cin >> efficiency;

        double mass;
        cout << "Enter Gearbox Mass (kg): ";
        cin >> mass;

        double diameter;
        cout << "Enter Gearbox Diameter (mm): ";
        cin >> diameter;

        double width;
        cout << "Enter Gearbox Width (mm): ";
        cin >> width;


        Gearbox newGearbox(name, reductionRatio, mass, diameter, width, efficiency);

        // Push it into the vector
        gearboxes.push_back(newGearbox);

        // Ask if they want to add another
        cout << "\nDo you want to add another gearbox? (y/n): ";
        char choice;
        cin >> choice;

        if (choice != 'y') {
            break;
        }
    }
}



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


int main()
{
    int materialIndex = chooseMaterial();
    Material selected = materials[materialIndex];
    rectangle T1 ;
    circle C1 ;
    string x ;
    while (true) {
        cout << "\nEnter cross-section type (circle/rectangle): ";
        cin >> x;
        if (x == "circle" || x == "Circle" || x == "c") {
            handleC(selected, C1);
            break;
        }
        else if (x == "rectangle" || x == "Rectangle" || x == "r") {
            handleR(selected, T1);
            break;
        }
        else {
            cout << "Invalid input. Please enter 'circle' or 'rectangle'.\n";
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
