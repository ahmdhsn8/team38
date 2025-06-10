#define _USE_MATH_DEFINES  // 34an el pi t4ta8al
#include<iostream>
#include<cmath>
#include <vector>
#include <limits>
#include <string>
#include <regex>

using namespace std;

const double g = 9.81; //m/s2
class link;

char validchar (string y)
{
    char value;
    while (true)
    {
        cout << y;
        cin >> value;
        if (value =='y'||value =='n'||value =='Y'||value =='N')
        {
            break;
        }
        else
        {
            cout << "Invalid input. Please enter (y/n) \n " ;
            cin.clear(); // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return value;
}
double ValidDouble(string prompt)   // Function to validate and read a double input
{
    string input;
    regex validPattern("^[0-9]+(\\.[0-9]+)?$"); // Only numbers and optional decimal
    double value;

    while (true)
    {
        cout << prompt;
        cin >> input;

        if (regex_match(input, validPattern))
        {
            value = stod(input);
            if (value > 0)
            {

                return value;
                break;
            }

        }

        cout << "Invalid input. Please enter a positive number with no special characters.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
int ValidInt(int min, int max) // Function to validate and read an integer input
{
    string input;
    regex validPattern("^[0-9]+$");   // Only numbers
    int value;
    while (true)
    {
        cout << "Enter a number between " << min << " and " << max << ": ";
        cin >> input;
        if (regex_match(input, validPattern))
        {
            value = stoi(input);
            if (value >= min && value <= max)
            {
                return value;
                break;
            }

        }

        cout << "Invalid input. Please enter a valid integer within range.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string ValidString(string prompt)
{
    string value;
    regex validPattern("^[A-Za-z0-9]+$");  // Only alphanumeric characters

    while (true)
    {
        cout << prompt;
        getline(cin >> ws, value);

        if (regex_match(value, validPattern))
        {
            return value;
        }
        else
        {
            cout << "Invalid input. Please avoid special characters.\n";
        }
    }
}
// Function to validate and read a string input

class Material
{
protected:
    string name;
    double yield_strength,density;
public:
    //function constructor bta3 input l material
    Material(string name, double yield_strength, double density)
    {
        this-> name = name;
        this-> yield_strength= yield_strength;
        this-> density=density;
    }
    //fn output l properties
    void display_material_properties() const
    {
        cout<<"Material: "<<name;
        cout<<"\nYield Strength: "<<yield_strength<<" Mpa\n";
        cout<<"Density: "<<density<<" g/cm³\n";
        double chosen_yield =yield_strength;
    }
    string getName() const
    {
        return name;
    }
    double getDensity()const
    {
        return density;
    }
    double getYieldStrength() const
    {
        return yield_strength;
    }
};

class link
{
private:
    /*
    h: height | b: base | r: radius | l: Length | m: idk | p: density
    mP: mass payload | alphaMax: max angular acc | yield: yield strength | stepRatio: idk
    */
    double h, b, r, l, p, mP, alphaMax, yield, stepRatio, safetyFactor; //justify why safety factor is double not float
    string crossSectionShape;
public:

    //Constructor
    link(double h =0, double b =0, double r =0, double l=0, double p =0, double mP=0, double alphaMax=0, double yield =0, double stepRatio=0, double safetyFactor = 100)
    {
        this-> h=h;
        this-> b=b;
        this-> r=r;
        this-> l=l;
        this-> p=p;
        this-> mP=mP;
        this-> alphaMax=alphaMax;
        this-> yield=yield;
        this-> stepRatio=stepRatio;
        this-> safetyFactor=safetyFactor;
    }

    void handlingLinkShape(Material selected, link &L1)
    {
        if (this->crossSectionShape == "circle")
        {
            handleCircle(selected, L1);
        }
        else if (this->crossSectionShape == "rectangle")
        {
            handleRectangle(selected, L1);
        }
    }

    //getter functions
    double getmP()
    {
        return mP;
    }
    double getl()
    {
        return l;
    }
    double getalphaMax()
    {
        return alphaMax;
    }
    double getsafetyFactor()
    {
        return safetyFactor;
    }

    //main link properties function
    //Rectangle Properties
    long double AreaRec()
    {
        return h*b ;
    }
    long double InertiaRec()
    {
        return (b * pow(h, 3)) / 12.0;
    }
    long double MaxStressRec()
    {
        return (bendingMomentRec()*1000*h)/(2*InertiaRec());
    }
    //Rectangle
    long double MassRec()
    {
        return p*b*h*l*pow(10,-6);
    }
    long double bendingMomentRec()
    {
        return MassRec()*9.81*l*pow(10,-3)*0.5 + mP*9.81*l*pow(10,-3) + (MassRec() *pow((0.5*l*pow(10,-3)),2) *alphaMax + mP*pow(l*pow(10,-3),2)*alphaMax);
    }

    //Circle Properties
    long double AreaCirc()
    {
        return r*r*M_PI ; //gives mm^2
    }
    long double InertiaCirc()
    {
        return (M_PI * pow(r, 4)) / 4.0; //gives mm^4
    }
    long double MaxStressCirc()
    {
        return (bendingMomentCirc()*1000*r)/InertiaCirc(); //gives MPa
    }
    long double MassCirc()
    {
        return p*M_PI*pow(r,2)*l*pow(10,-6); // gives kilogram
    }
    long double bendingMomentCirc()
    {
        return (MassCirc()*9.81*l*0.5*pow(10,-3) + mP*9.81*l*pow(10,-3) + (MassCirc() *pow((0.5*l*pow(10,-3)),2) *alphaMax + mP*pow(l*pow(10,-3),2)*alphaMax));
    }    // gives N.m

    //Selection of Cross Section
    void crossSectionSelection()
    {
        string x;
        while (true)
        {
            cout << "\nEnter cross-section type (circle/rectangle): ";
            cin >> x;
            if (x == "circle" || x == "Circle" || x == "c")
            {
                crossSectionShape = "circle";
                break;
            }
            else if (x == "rectangle" || x == "Rectangle" || x == "r")
            {
                crossSectionShape = "rectangle";
                break;
            }
            else
            {
                cout << "Invalid input. Please enter 'circle' or 'rectangle'.\n";
            }
        }
    }

    //Handling Functions For Call after selecting Material
    //Rectangle Handling Function
    void handleRectangle(const Material& selected, link& T1)
    {
        h = ValidDouble("\nRectangle height (mm): ");
        b = ValidDouble("Rectangle width (mm): ");
        l = ValidDouble("Member length (mm): ");
        p = selected.getDensity();
        yield = selected.getYieldStrength();
        mP = ValidDouble("Payload (kg): ");
        alphaMax = ValidDouble("Max angular acceleration (rad/s^2): ");
        stepRatio = ValidDouble("Step ratio (%) [default 1%]: ");
        safetyFactor = ValidDouble("Safety factor (%) [default 100% from yield]: ");
        flow_func_rec(T1);
        cout << "\n--- Optimization Complete ---\n"
             << "Final Height: " << h << " mm\n"
             << "Final Width: " << b << " mm\n"
             << "Final Stress: " << MaxStressRec() << " MPa\n"
             << "Bending Moment: " << bendingMomentRec() << " Nm\n"
             << "Mass: " << MassRec() << " kg\n";
    }
    //Circle Handling Function
    void handleCircle(const Material& selected, link& C1)
    {
        r = ValidDouble("\nCircle radius (mm): ");
        l = ValidDouble("Member length (mm): ");
        p = selected.getDensity();
        yield = selected.getYieldStrength();
        mP = ValidDouble("Payload (kg): ");
        alphaMax = ValidDouble("Max angular acceleration (rad/s^2): ");
        stepRatio = ValidDouble("Step ratio (%) [default 1%]: ");
        safetyFactor = ValidDouble("Safety factor (%) [default 100% from yield]: ");
        flow_func_circ("Circle", C1);

        cout << "\n--- Optimization Complete ---\n"
             << "Final Radius: " << r << " mm\n"
             << "Final Stress: " << MaxStressCirc() << " MPa\n"
             << "Bending Moment: " << bendingMomentCirc() << " Nm\n"
             << "Mass: " << MassCirc() << " kg\n";
    }

    //Flow Function For the iterative logic of the program
    //Rectangle Flow Function
    void flow_func_rec(link & T)  //hanwsal l7d as8r aw akbar mn sigma yield b 0.1
    {
        double sigma_calc= MaxStressRec();
        double sigma_yield= yield;
        long long int max_iter= pow(10,10);
        int iter=0;
        double stepRatio =1;

        if (sigma_calc < sigma_yield)
        {
            while (sigma_calc < (safetyFactor/100 * sigma_yield) && iter < max_iter)
            {
                b -= stepRatio /100 * b;
                h -= stepRatio /100 * h;
                sigma_calc = MaxStressRec();
                iter++;
            }
        }
        else if (sigma_calc > sigma_yield )
        {
            while (sigma_calc > (safetyFactor/100  * sigma_yield)  && iter < max_iter )
            {
                b += stepRatio /100 * b;
                h += stepRatio /100 * h;
                sigma_calc = MaxStressRec();
                iter++;
            }
        }
        cout << "\nNumber of iterations = " << iter << "\n";
        if (iter >= max_iter)
        {
            cout << "\n Optimization failed: reached max iterations.\n";
        }
    }

    //Circle Flow Function
    void flow_func_circ(const string shapeType, link & C)  //hanwsal l7d as8r aw akbar mn sigma yield b 0.1
    {
        double sigma_calc= MaxStressCirc();
        double sigma_yield= yield;
        long long int max_iter=pow(10,10);
        int iter=0;
        double stepRatio =1;

        if (shapeType=="Circle" && sigma_calc < sigma_yield)
        {
            while (sigma_calc < (safetyFactor/100 * sigma_yield)  ) //&& iter < max_iter
            {
                r -= stepRatio /100 * r;
                sigma_calc = MaxStressCirc();
                iter++;
            }
        }
        else if (shapeType=="Circle" && sigma_calc > sigma_yield )
        {
            while (sigma_calc > (safetyFactor/100 * sigma_yield)  ) //&& iter < max_iter
            {
                r += stepRatio /100 * r;
                sigma_calc = MaxStressCirc();
                iter++;
            }
        }
        cout <<"\n number of iteration = "<<iter<<"\n";
        if (iter >= max_iter)
        {
            cout << "\n Optimization failed: reached max iterations.\n";
        }
    }
};

class Motor
{
public:
    double torque,speed,mass,diameter,width;
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
    string getName()
    {
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
        cout << "Efficency: "<< eff*100<< " %\n";
    }

};

class Pairs
{
public:
    Motor* M_REF;
    Gearbox* G_REF;
    float Tout,Wout,cost,Mtotal,Dtotal,Wtotal;
    Pairs() {}
    Pairs(Motor &MID, Gearbox &GID)
    {
        this -> M_REF = &MID;
        this -> G_REF = &GID;
    }

};

vector <Material> materials =
{
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

// List of all motors
vector<Motor> motors
{
    Motor("Motor 1", 0.322, 6650, 225, 50, 22),
    Motor("Motor 2", 1.710, 3410, 741, 85, 33),
    Motor("Motor 3", 0.430, 4330, 270, 50, 27),
    Motor("Motor 5", 0.688, 4570, 377, 65, 25),
    Motor("Motor 6", 1.130, 2590, 524, 65, 33),
    Motor("Motor 7", 0.186, 6290, 170, 38, 25),
    Motor("Motor 8", 0.097, 12400, 125, 38, 19)
};

// List of all gearboxes
vector<Gearbox> gearboxes
{
    Gearbox("GB 12 worm gear", 30/1, 1.5, 12, 62, 65),
    Gearbox("Planetary gearhead GP 42 A", 1296/1, .56,42,155.6,64),
    Gearbox("Planetary Gearhead GP 16 A Ø16 mm, Sleeve Bearing", 4.4/1, .02, 16, 52.3, .9)
};

vector <Pairs> PairsV;

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

void getRequiredPairs(double Treq, double Wreq)
{
    for (int i = 0; i < motors.size(); i++)
    {
        for (int j = 0; j < gearboxes.size(); j++)
        {
            double Tout = torqueMotorGear(motors[i].torque, gearboxes[j].redRatio, gearboxes[j].eff);
            double Wout = speedMotorGear(motors[i].speed, gearboxes[j].redRatio);
            if((Tout >= Treq) && (Wout >= Wreq))
            {
                Pairs tempPair;
                tempPair.M_REF = &motors[i];
                tempPair.G_REF = &gearboxes[j];
                PairsV.push_back(tempPair);
            }
        }
    }
}

void printPairs(vector <Pairs> &PairsV)
{
    if(PairsV.size() == 0)
    {
        cout << "No Pairs capable of lifting this mass!"<<endl;
    }
    else
    {
        for (int j = 0; j < PairsV.size(); j++)
        {
            cout << "Pair No {" << j+1 << "}: " << PairsV[j].M_REF->getName() << " with " << PairsV[j].G_REF->name << endl;
        }
    }
}

vector <Pairs> Edit_dimensions(vector <Pairs> &PairsV)
{
    char answer;
    answer = validchar ( "\nfilter motors and grarboxs by equal diameters? (y/n): " ) ;

    if ( answer == 'y' || answer == 'Y' )
    {
        vector <Pairs> sameDimesion_pairs;
        for (int i = 0; i < PairsV.size() ; i++)
        {
            // Check if the diameters of the motor and gearbox are equal
            // If they are, add the pair to the sameDimesion_pairs vector
            if (PairsV[i].M_REF->diameter == PairsV[i].G_REF->diameter)
            {
                sameDimesion_pairs.push_back(PairsV[i]);
            }
        }
        return sameDimesion_pairs;
    }
    // If the user does not want to filter by equal diameters
    else if (answer == 'n' || answer == 'N')
    {
        cout << "[*] The diameters for motor and gearbox will not be filtered.\n";
        return PairsV;
    }
}

int chooseMaterial()
{
    cout << "Choose a material:\n";
    for (int i = 0; i < materials.size(); i++)
        cout << i + 1 << "- " << materials[i].getName() << "\n";
    cout << materials.size() + 1 << "- New Material\n";

    int choice = ValidInt(1, materials.size() + 1);

    if (choice == materials.size() + 1)
    {
        string newName = ValidString("New Material Name: ");
        double newYield = ValidDouble("Yield Strength (MPa): ");
        double newDensity = ValidDouble("Density (g/cm^3): ");
        materials.emplace_back(newName, newYield, newDensity);
        choice = materials.size();
    }
    materials[choice - 1].display_material_properties();
    return choice - 1;
}

// the user keeps adding motors as much as he wants
void adding_motors()//ha7tag a7ot adding_motors(); fel main ... matensash //
{
    while (true)
    {

        cout << "\nAdding a new Motor:\n";

        string name = ValidString("Enter Motor Name: ");

        double torque=ValidDouble ("Enter Motor Torque (mNm): ") ;

        double speed =ValidDouble ("Enter Motor Speed (rpm): ") ;

        double mass = ValidDouble ("Enter Motor Mass (kg): ") ;

        double diameter =ValidDouble ("Enter Motor Diameter (mm): ") ;

        double width =ValidDouble ("Enter Motor Width (mm): ") ;



        Motor newMotor(name, torque, speed, mass, diameter, width);

        // Push it into the vector
        motors.push_back(newMotor);

        // Ask if they want to add another
        char choice = validchar ("\nDo you want to add another motor? (y/n): ");
        if (choice == 'n' || choice == 'N')
        {
            cout << "Exiting Motor addition.\n";
            break;
        }
        else if (choice == 'y' || choice == 'Y')
        {
            cout << "Adding another motor...\n";
        }
    }
}
// the user keeps adding gearboxes as much as he wants
void adding_gearboxes()//ha7tag a7ot adding_gearboxes(); fel main ... matensash //
{

    while (true)
    {
        cout << "\nAdding a new Gearbox:\n";

        string name = ValidString( "Enter Gearbox Name: ");

        double reductionRatio =ValidDouble ("Enter Gearbox Reduction Ratio: ") ;

        double efficiency =ValidDouble ("Enter Gearbox Efficiency (e.g., 0.85 for 85%): ") ;

        double mass =ValidDouble ("Enter Gearbox Mass (kg): ") ;

        double diameter =ValidDouble ("Enter Gearbox Diameter (mm): ") ;

        double width = ValidDouble ("Enter Gearbox Width (mm): ") ;

        Gearbox newGearbox(name, reductionRatio, mass, diameter, width, efficiency);

        // Push it into the vector
        gearboxes.push_back(newGearbox);

        // Ask if they want to add another
        char choice;
        choice = validchar ("\nDo you want to add another gearbox? (y/n): ") ;
        if (choice == 'N'|| choice == 'n')
        {
            cout << "Exiting Gearbox addition.\n";
            break;
        }
        else if (choice == 'y' || choice == 'Y')
        {
            cout << "Adding another gearbox...\n";
        }
    }
}

int main()
{
    link L1;

    int materialIndex = chooseMaterial();
    Material selected = materials[materialIndex];

    L1.crossSectionSelection();
    L1.handlingLinkShape(selected, L1);

    adding_motors();
    adding_gearboxes();

    double Treq = torqueRec(L1.MassCirc(), L1.getl()/1000, L1.getmP(), L1.getalphaMax());
    double Wreq = ValidDouble("What is your required speed? [rpm]: ")*2*M_PI/60;

    getRequiredPairs(Treq, Wreq);

    PairsV = Edit_dimensions(PairsV);
    printPairs(PairsV);

}
