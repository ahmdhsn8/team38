#define _USE_MATH_DEFINES  // 34an el pi t4ta8al
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <string>
#include <regex>

using namespace std;

const double g = 9.81; //m/s^2
class link;
double Treq;
double Wreq;

char validchar (string y) // Function to validate and read a character input
{
    char value; // Variable to store the input character
    while (true) // Loop until a valid input is received
    {
        cout << y;
        cin >> value;
        if (value =='y'||value =='n'||value =='Y'||value =='N') // Check if the input is either 'y' or 'n' (case insensitive)
        {
            break;
        }
        else // If the input is not valid, prompt the user again
        {
            cout << "[!] Invalid input. Please enter (y/n) \n " ;
            cin.clear(); // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore the rest of the line
            // This will ignore the rest of the line until a newline character is found
        }
    }
    return value;
}
double ValidDouble(string prompt)   // Function to validate and read a double input
{
    string input; // Variable to store the input string
    regex validPattern("^[0-9]+(\\.[0-9]+)?$"); // Only numbers and optional decimal
    double value; // Variable to store the converted double value

    while (true) // Loop until a valid input is received
    {
        cout << prompt; // Prompt the user for input
        cin >> input; // Read the input from the user

        if (regex_match(input, validPattern)) // Check if the input matches the valid pattern
        {
            value = stod(input);
            if (value > 0) // Check if the value is positive
            {

                return value;
                break;
            }

        }

        cout << "[!] Invalid input. Please enter a positive number with no special characters.\n";
        cin.clear(); // Clear the error flag on cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Ignore the rest of the line until a newline character is found
    }
}
int ValidInt(int min, int max) // Function to validate and read an integer input
{
    string input; // Variable to store the input string
    regex validPattern("^[0-9]+$");   // Only numbers
    int value; // Variable to store the converted integer value
    while (true) // Loop until a valid input is received
    {
        cout << "[+] Enter a number between " << min << " and " << max << ": ";
        cin >> input;
        if (regex_match(input, validPattern)) // Check if the input matches the valid pattern
        {
            value = stoi(input); // Convert the input string to an integer
            if (value >= min && value <= max) // Check if the value is within the specified range
            {
                return value;
                break;
            }

        }

        cout << "[!] Invalid input. Please enter a valid integer within range.\n";
        cin.clear(); // Clear the error flag on cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line until a newline character is found
    }
}
string ValidString(string prompt) // Function to validate and read a string input
{
    string value; // Variable to store the input string
    regex validPattern("^[A-Za-z0-9]+$");  // Only alphanumeric characters

    while (true) // Loop until a valid input is received
    {
        cout << prompt; // Prompt the user for input
        getline(cin >> ws, value); // Read the input from the user, ignoring leading whitespace

        if (regex_match(value, validPattern)) // Check if the input matches the valid pattern
        {
            return value;
        }
        else
        {
            cout << "Invalid input. Please avoid special characters.\n";
        }
    }
}

//Torque calculation
//Required Torque Calculation
double torqueRec(double ml, double l, double mp, double sig)
{
    //check for overflows
    return ml*9.81*l*0.5+mp*9.81*l+ml*pow((l*0.5),2)*sig +mp*pow(l,2)*sig;
}
//Motor gear box output tourque calculation
double torqueMotorGear(double tMotor, double ratio, double efficency)
{
    return tMotor*ratio*efficency;
}

class Material   // Class to represent a material with its properties
{
protected:
    string name;
    double yield_strength,density;
public:
    Material(string name, double yield_strength, double density) // Constructor to initialize the material properties
    {
        this-> name = name;
        this-> yield_strength= yield_strength;
        this-> density=density;
    }
    void display_material_properties() const // Function to display the material properties
    {

        cout<<"\n| Material: " << name;
        cout<<"\n| Yield Strength: " << yield_strength << " Mpa";
        cout<<"\n| Density: "<<density<<" g/cm^3\n";
        double chosen_yield =yield_strength;
    }

    string getName() const // Function to get the name of the material
    {
        return name;
    }
    double getDensity()const // Function to get the density of the material
    {
        return density;
    }
    double getYieldStrength() const // Function to get the yield strength of the material
    {
        return yield_strength;
    }
};

class link // Class to represent a link with its properties and methods for calculations
{
private:
    /*
    h: height | b: base | r: radius | l: Length | p: density
    mP: mass payload | alphaMax: max angular acc | yield: yield strength |
    stepRatio: ratio of increment or decrement in optimization
    */
    double h, b, r, l, p, mP, alphaMax, yield, stepRatio, safetyFactor; //justify why safety factor is double not float
    string crossSectionShape,materialName;
public:
    // Constructor to initialize the link properties
    link(double h =0, double b =0, double r =0, double l=0, double p =0, double mP=0, double alphaMax=0, double yield =0, double stepRatio=1, double safetyFactor = 100)
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
    double getmP() // Function to get the mass payload
    {
        return mP;
    }
    double getl() // Function to get the length of the link
    {
        return l;
    }
    double getalphaMax() // Function to get the maximum angular acceleration
    {
        return alphaMax;
    }
    double getsafetyFactor() // Function to get the safety factor
    {
        return safetyFactor;
    }

    //main link properties function
    //Rectangle Properties
    long double AreaRec()  // Area in mm^2
    {
        return h*b ;
    }
    long double InertiaRec()  // Inertia in mm^4
    {
        return (b * pow(h, 3)) / 12.0;
    }
    long double MaxStressRec()  // Maximum Stress in MPa
    {
        return (bendingMomentRec()*1000*h)/(2*InertiaRec());
    }
    //Rectangle
    long double MassRec()  // Link mass in kg
    {
        return p*b*h*l*pow(10,-6);
    }
    long double bendingMomentRec()  // Bending Moment in N.m
    {
        return MassRec()*9.81*l*pow(10,-3)*0.5 + mP*9.81*l*pow(10,-3) + (MassRec() *pow((0.5*l*pow(10,-3)),2) *alphaMax + mP*pow(l*pow(10,-3),2)*alphaMax);
    }

    //Circle Properties
    long double AreaCirc()
    {
        return r*r*M_PI ; // Area in mm^2
    }
    long double InertiaCirc()
    {
        return (M_PI * pow(r, 4)) / 4.0; // Inertia in mm^4
    }
    long double MaxStressCirc()
    {
        return (bendingMomentCirc()*1000*r)/InertiaCirc(); // Maximum Stress in MPa
    }
    long double MassCirc()
    {
        return p*M_PI*pow(r,2)*l*pow(10,-6); // Mass in kilogram
    }
    long double bendingMomentCirc()    // Bending Moment in N.m
    {
        return (MassCirc()*9.81*l*0.5*pow(10,-3) + mP*9.81*l*pow(10,-3) + (MassCirc() *pow((0.5*l*pow(10,-3)),2) *alphaMax + mP*pow(l*pow(10,-3),2)*alphaMax));
    }

    //Selection of Cross Section
    void crossSectionSelection() // Function to select the type of cross-section
    {
        string x; // Variable to store user input
        while (true) // Loop until a valid input is received
        {
            cout << "\n[+] Enter cross-section type (circle/rectangle): ";
            cin >> x;
            if (x == "circle" || x == "Circle" || x == "c") // Check for circle input
            {
                crossSectionShape = "circle";
                break;
            }
            else if (x == "rectangle" || x == "Rectangle" || x == "r") // Check for rectangle input
            {
                crossSectionShape = "rectangle";
                break;
            }
            else // If the input is not valid, prompt the user again
            {
                cout << "[!] Invalid input. Please enter 'circle' or 'rectangle'.\n";
            }
        }
    }

    //Handling Functions For Call after selecting Material
    //Rectangle Handling Function
    void handleRectangle(const Material& selected, link& T1)
    {
        h = ValidDouble("\n[+] Rectangle height (mm): ");
        b = ValidDouble("[+] Rectangle width (mm): ");
        l = ValidDouble("[+] Member length (mm): ");
        p = selected.getDensity();
        yield = selected.getYieldStrength();
        materialName = selected.getName();
        mP = ValidDouble("[+] Payload (kg): ");
        alphaMax = ValidDouble("[+] Max angular acceleration (rad/s^2): ");
        Treq = torqueRec(T1.MassRec(), l/1000, mP, alphaMax);
        Wreq = ValidDouble("[+] What is your required speed? [rpm]: ")*2*M_PI/60;
        stepRatio = ValidDouble("[+] Step ratio (%) [default 1%]: ");
        safetyFactor = ValidDouble("[+] Safety factor (%) [default 100% from yield]: ");
        flow_func_rec(T1);
        cout << "\n--- Optimization Complete ---\n"
             << "| Final Height: " << h << " mm\n"
             << "| Final Width: " << b << " mm\n"
             << "| Final Stress: " << MaxStressRec() << " MPa\n"
             << "| Bending Moment: " << bendingMomentRec() << " Nm\n"
             << "| Mass: " << MassRec() << " kg\n";
    }
    //Circle Handling Function
    void handleCircle(const Material& selected, link& C1)
    {
        r = ValidDouble("\n[+] Circle radius (mm): ");
        l = ValidDouble("[+] Member length (mm): ");
        p = selected.getDensity();
        yield = selected.getYieldStrength();
        materialName = selected.getName();
        mP = ValidDouble("[+] Payload (kg): ");
        alphaMax = ValidDouble("[+] Max angular acceleration (rad/s^2): ");
        Treq = torqueRec(C1.MassCirc(), l/1000, mP, alphaMax);
        Wreq = ValidDouble("[+] What is your required speed? [rpm]: ")*2*M_PI/60;
        stepRatio = ValidDouble("[+] Step ratio (%) [default 1%]: ");
        safetyFactor = ValidDouble("[+] Safety factor (%) [default 100% from yield]: ");
        flow_func_circ(C1);

        cout << "\n--- Optimization Complete ---\n"
             << "| Final Radius: " << r << " mm\n"
             << "| Final Stress: " << MaxStressCirc() << " MPa\n"
             << "| Bending Moment: " << bendingMomentCirc() << " Nm\n"
             << "| Mass: " << MassCirc() << " kg\n";
    }

    //Flow Function For the iterative logic of the program
    //Rectangle Flow Function
    void flow_func_rec(link &T)
    {
        double sigma_calc = MaxStressRec();
        double allowable = (safetyFactor / 100.0) * yield;
        double tolerance = 0.01; // 0.01 MPa tolerance
        long int max_iter = 100000;
        int iter = 0;
        while (abs(sigma_calc - allowable) > tolerance && iter < max_iter)
        {
            if (sigma_calc > allowable)
            {
                b += (stepRatio / 100.0) * b;
                h += (stepRatio / 100.0) * h;
            }
            else
            {
                b -= (stepRatio / 100.0) * b;
                h -= (stepRatio / 100.0) * h;
            }

            // Prevent unrealistic dimensions
            if (b < 1.0 || h < 1.0)
            {
                cout << "\n[!] Rectangle dimensions too small. Optimization aborted.\n";
                break;
            }
            sigma_calc = MaxStressRec();
            iter++;
        }
        if (iter >= max_iter)
        {
            cout << "\n[!] Rectangle optimization failed: max iterations reached.\n";
        }
    }
    //Circle Flow Function
    void flow_func_circ(link &C)
    {
        double sigma_calc = MaxStressCirc();
        double allowable = (safetyFactor / 100.0) * yield;
        double tolerance = 0.01; // 0.01 MPa tolerance
        long int max_iter = 100000;
        int iter = 0;

        while (abs(sigma_calc - allowable) > tolerance && iter < max_iter)
        {
            if (sigma_calc > allowable)
            {
                r += (stepRatio / 100.0) * r;
            }
            else
            {
                r -= (stepRatio / 100.0) * r;
            }
            // Prevent unrealistic diameter
            if (r < 1.0)
            {
                cout << "\n[!] Circle diameter too small. Optimization aborted.\n";
                break;
            }
            sigma_calc = MaxStressCirc();
            iter++;
        }
        if (iter >= max_iter)
        {
            cout << "\n[!] Circle optimization failed: max iterations reached.\n";
        }
    }

    void finalResults() // Function to print final link dimension results
    {
        if (this->crossSectionShape == "circle")
        {
            cout << "\n--- Final Results ---\n"
                 << "| Material Selected: " << materialName <<"\n"
                 << "| Final Radius: " << r << " mm\n"
                 << "| Final Stress: " << MaxStressCirc() << " MPa\n"
                 << "| Bending Moment: " << bendingMomentCirc() << " Nm\n"
                 << "| Mass: " << MassCirc() << " kg\n";
        }
        else if (this->crossSectionShape == "rectangle")
        {
            cout << "\n--- Final Results ---\n"
                 << "| Material Selected: " << materialName <<"\n"
                 << "| Final Height: " << h << " mm\n"
                 << "| Final Width: " << b << " mm\n"
                 << "| Final Stress: " << MaxStressRec() << " MPa\n"
                 << "| Bending Moment: " << bendingMomentRec() << " Nm\n"
                 << "| Mass: " << MassRec() << " kg\n";
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
    string getName() const // Function to get the name of the motor
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
    void allCalculations()
    {
        Tout = M_REF->torque * G_REF->redRatio * G_REF->eff;
        Wout = M_REF->speed / G_REF->redRatio * G_REF->eff;
        Mtotal = M_REF-> mass + G_REF-> mass;
        Dtotal = M_REF-> diameter + G_REF-> diameter;
        Wtotal = M_REF-> width + G_REF-> width;
        cost = Mtotal + Dtotal/100 + Wtotal/100;
    }

};

vector <Material> materials = // List of all materials with their properties
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
    Gearbox("Planetary Gearhead GP 16 A Sleeve Bearing", 4.4/1, .02, 16, 52.3, .9)
};

vector <Pairs> PairsV;

//Motor-Gearbox output speed calculation
double speedMotorGear(double w, double ratio )
{
    return w/ratio;
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
                tempPair.allCalculations();
                PairsV.push_back(tempPair);
            }
        }
    }
}

void printPairs(vector <Pairs> &PairsV)
{
    if(PairsV.size() == 0)
    {
        cout << "[!] No Pairs capable of lifting this mass!"<<endl;
    }
    else
    {
        cout << "[*] All acceptable pairs:\n";
        for (int j = 0; j < PairsV.size(); j++)
        {
            cout << "| Pair No {" << j+1 << "} | " << "costs: "<< PairsV[j].cost << " consists of: {"<< PairsV[j].M_REF->getName() << "} with {" << PairsV[j].G_REF->name << "}" << endl;
        }
    }
}

vector <Pairs> Edit_dimensions(vector <Pairs> &PairsV)
{
    char answer;
    answer = validchar ( "\n[+] filter motors and grarboxs by equal diameters? (y/n): " ) ;

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
        return PairsV;
    }
}

int chooseMaterial()
{
    cout << "[*] Choose a material:\n";
    for (int i = 0; i < materials.size(); i++)
        cout << i + 1 << "- " << materials[i].getName() << "\n";
    cout << materials.size() + 1 << "- New Material\n";

    int choice = ValidInt(1, materials.size() + 1);

    if (choice == materials.size() + 1)
    {
        string newName = ValidString("[+] New Material Name: ");
        double newYield = ValidDouble("[+] Yield Strength (MPa): ");
        double newDensity = ValidDouble("[+] Density (g/cm^3): ");
        materials.emplace_back(newName, newYield, newDensity);
        choice = materials.size();
    }
    materials[choice - 1].display_material_properties();
    return choice - 1;
}

// the user keeps adding motors as much as he wants
void adding_motors()//ha7tag a7ot adding_motors(); fel main ... matensash //
{
    char choice = validchar("\n[+] Do you want to add a new motor? (y/n): ");
    if (choice == 'Y' || choice == 'y')
    {
        while (true)
        {
            cout << "\n[*] Adding a new Motor:\n";

            string name = ValidString("[+] Enter Motor Name: ");
            double torque=ValidDouble ("[+] Enter Motor Torque (mNm): ") ;
            double speed =ValidDouble ("[+] Enter Motor Speed (rpm): ") ;
            double mass = ValidDouble ("[+] Enter Motor Mass (kg): ") ;
            double diameter =ValidDouble ("[+] Enter Motor Diameter (mm): ") ;
            double width =ValidDouble ("[+] Enter Motor Width (mm): ") ;

            Motor newMotor(name, torque, speed, mass, diameter, width);
            // Push it into the vector
            motors.push_back(newMotor);

            // Ask if they want to add another
            choice = validchar ("\n[+] Do you want to add another motor? (y/n): ");
            if (choice == 'n' || choice == 'N')
            {
                cout << "[*] Exiting Motor addition.\n";
                break;
            }
            else if (choice == 'y' || choice == 'Y')
            {
                cout << "[*] Adding another motor...\n";
            }
        }
    }
    else
    {
        cout << "[*] You are using the database now." << endl;
    }
}
// the user keeps adding gearboxes as much as he wants
void adding_gearboxes()//ha7tag a7ot adding_gearboxes(); fel main ... matensash //
{

    char choice = validchar("\n[+] Do you want to add a new gearbox? (y/n): ");
    if (choice == 'Y' || choice == 'y')
    {
        while (true)
        {
            cout << "\n[*] Adding a new Gearbox:\n";

            string name = ValidString("[+] Enter Gearbox Name: ");
            double reductionRatio =ValidDouble ("[+] Enter Gearbox Reduction Ratio: ") ;
            double efficiency =ValidDouble ("[+] Enter Gearbox Efficiency (e.g., 0.85 for 85%): ") ;
            double mass =ValidDouble ("[+] Enter Gearbox Mass (kg): ") ;
            double diameter =ValidDouble ("[+] Enter Gearbox Diameter (mm): ") ;
            double width = ValidDouble ("[+] Enter Gearbox Width (mm): ") ;

            Gearbox newGearbox(name, reductionRatio, mass, diameter, width, efficiency);

            // Push it into the vector
            gearboxes.push_back(newGearbox);

            // Ask if they want to add another
            choice = validchar ("\n[+] Do you want to add another gearbox? (y/n): ") ;
            if (choice == 'N'|| choice == 'n')
            {
                cout << "[*] Exiting Gearbox addition.\n";
                break;
            }
            else if (choice == 'y' || choice == 'Y')
            {
                cout << "[*] Adding another gearbox...\n";
            }
        }
    }
    else
    {
        cout << "[*] You are using the database now." << endl;
    }

}

void sortingPairs(vector <Pairs> &PairsV)
{
    string choice;
    while(true)
    {
        choice = ValidString("\n[+]Do you want to sort the output based on \"Cost\" or \"Torque\" or \"Speed\": ");
        if (!(choice == "cost" || choice == "Cost" || choice == "Torque" || choice == "torque" || choice == "Speed" || choice == "speed" ))
        {
            cout << "[!] Invalid input, please type \"Cost\" or \"Torque\" or \"Speed\""<< endl;
        }
        else
        {
            break;
        }
    }

    //Sorting
    int n = PairsV.size();
    if(choice == "cost" || choice == "Cost")
    {
        cout << "[*] Sorting Based on Cost (Least to Greatest)" << endl;
        // Outer loop that corresponds to the number of elements to be sorted
        for (int i = 0; i < n - 1; i++)
        {
            // Last i elements are already in place
            for (int j = 0; j < n - i - 1; j++)
            {
                if (PairsV[j+1].cost < PairsV[j].cost)
                {
                    swap(PairsV[j], PairsV[j + 1]);
                }
            }
        }
        printPairs(PairsV);
    }
    else if(choice == "torque" || choice == "Torque")
    {
        cout << "[*] Sorting Based on Torque (Greatest to Least)" << endl;
        // Outer loop that corresponds to the number of elements to be sorted
        for (int i = 0; i < n - 1; i++)
        {
            // Last i elements are already in place
            for (int j = 0; j < n - i - 1; j++)
            {
                if (PairsV[j+1].Tout > PairsV[j].Tout)
                {
                    swap(PairsV[j], PairsV[j + 1]);
                }
            }
        }
        cout << "[*] All acceptable pairs:\n";
        for (int j = 0; j < PairsV.size(); j++)
        {
            cout << "| Pair No {" << j+1 << "} | " << "Torque: "<< PairsV[j].Tout << " consists of: {"<< PairsV[j].M_REF->getName() << "} with {" << PairsV[j].G_REF->name << "}" << endl;
        }
    }
    else if(choice == "speed" || choice == "Speed")
    {
        cout << "[*] Sorting Based on Speed (Greatest to Least)" << endl;
        // Outer loop that corresponds to the number of elements to be sorted
        for (int i = 0; i < n - 1; i++)
        {
            // Last i elements are already in place
            for (int j = 0; j < n - i - 1; j++)
            {
                if (PairsV[j+1].Wout > PairsV[j].Wout)
                {
                    swap(PairsV[j], PairsV[j + 1]);
                }
            }
        }
        cout << "[*] All acceptable pairs:\n";
        for (int j = 0; j < PairsV.size(); j++)
        {
            cout << "| Pair No {" << j+1 << "} | " << "Speed: "<< PairsV[j].Wout << " consists of: {"<< PairsV[j].M_REF->getName() << "} with {" << PairsV[j].G_REF->name << "}" << endl;
        }
    }
}

void finalResults(vector <Pairs> &PairsV)
{
    if(PairsV.size() != 0)
    {
        vector <Pairs> bestCostPairs = {PairsV[0]};
        vector <Pairs> bestTorquePairs = {PairsV[0]};
        vector <Pairs> bestSpeedPairs = {PairsV[0]};

        for (int j = 0; j < PairsV.size(); j++)
        {
            //Getting the Greatest Torque
            if (PairsV[j].Tout > bestTorquePairs[0].Tout)
            {
                bestTorquePairs.clear();
                bestTorquePairs.push_back(PairsV[j]);
            }
            else if (PairsV[j].Tout == bestTorquePairs[0].Tout)
            {
                bestTorquePairs.push_back(PairsV[j]);
            }

            //Getting the least cost
            if (PairsV[j].cost < bestCostPairs[0].cost)
            {
                bestCostPairs.clear();
                bestCostPairs.push_back(PairsV[j]);
            }
            else if (PairsV[j].cost == bestCostPairs[0].cost)
            {
                bestCostPairs.push_back(PairsV[j]);
            }

            //Getting the Greatest Speed
            if (PairsV[j].Wout > bestSpeedPairs[0].Wout)
            {
                bestSpeedPairs.clear();
                bestSpeedPairs.push_back(PairsV[j]);
            }
            else if (PairsV[j].Wout == bestSpeedPairs[0].Wout)
            {
                bestSpeedPairs.push_back(PairsV[j]);
            }
        }
        cout << "[*] Least cost pair(s): \n";
        for (int j = 0; j < bestCostPairs.size(); j++)
        {
            cout << "| Pair No {" << j+1 << "} | " << "costs: "<< bestCostPairs[j].cost << " consists of: {"<< bestCostPairs[j].M_REF->getName() << "} with {" << bestCostPairs[j].G_REF->name << "}" << endl;
        }
        cout << "[*] Greatest Torque pair(s): \n";
        for (int j = 0; j < bestTorquePairs.size(); j++)
        {
            cout << "| Pair No {" << j+1 << "} | " << "Torque: "<< bestTorquePairs[j].Tout << " consists of: {"<< bestTorquePairs[j].M_REF->getName() << "} with {" << bestTorquePairs[j].G_REF->name << "}" << endl;
        }
        cout << "[*] Greatest Speed pair(s): \n";
        for (int j = 0; j < bestSpeedPairs.size(); j++)
        {
            cout << "| Pair No {" << j+1 << "} | " << "Speed: "<< bestSpeedPairs[j].Wout << " consists of: {"<< bestSpeedPairs[j].M_REF->getName() << "} with {" << bestSpeedPairs[j].G_REF->name << "}" << endl;
        }
    }
    sortingPairs(PairsV);
}

void thankYou()
{
    cout << "\n+-------------------------------+\n";
    cout  <<  "|...............................|\n";
    cout  <<  "|...Thanks for using our tool...|\n";
    cout  <<  "|...............................|\n";
    cout  <<  "+-------------------------------+";
}

int main()
{
    cout << "Welcome to Team 38 C++ project [Spring 25]\n" << endl;
    link L1;

    int materialIndex = chooseMaterial();
    Material selected = materials[materialIndex];

    L1.crossSectionSelection();
    L1.handlingLinkShape(selected, L1);

    adding_motors();
    adding_gearboxes();

    getRequiredPairs(Treq, Wreq);

    PairsV = Edit_dimensions(PairsV);
    printPairs(PairsV);

    L1.finalResults();
    finalResults(PairsV);
    thankYou();
}
