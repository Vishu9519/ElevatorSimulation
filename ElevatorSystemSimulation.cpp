#include <iostream>
#include <vector>
#include<ctime>
using namespace std;

class Floor;
class Rider
{
  const int RiderID; //Id for the rider
  static int copyID; //will store the number of riders.
  const Floor* const f; //constant pointer for constant object of Floor type
public:
  Rider& operator=(const Rider&);
  Rider(const Floor& x); //Constructor taking a constant reference of Floor object
  const Floor& getDestination() const{return *f;} //returns reference of destination floor
  bool operator==(const Rider&) const; //overloading the == operator
  bool operator<(const Rider&) const;
};

int Rider::copyID = 1;

Rider::Rider(const Floor& x):f(&x), RiderID(copyID) //assigning the address of the floor object x to f.
{
  copyID++;
}
bool Rider:: operator==(const Rider& b) const //will return true if the rider id of *this and the object b is same.
{
  bool result = false;
  if (RiderID== b.RiderID)result= true;

  return result;
}
bool Rider:: operator<(const Rider& b) const //will return true if current object's Rider id is lesser, i.e. Person came earlier
{
  bool result = false;
  if (RiderID< b.RiderID)result= true;

  return result;
}
Rider& Rider::operator=(const Rider& a) //making a rider same as the other rider
{ //const_cast overcomes the constant-ness of a constant data type.
  const_cast<int&>(RiderID) = a.RiderID; //We are doing const_cast so that we can change the value of RiderID through this reference.
  const_cast<const Floor*&>(f)=a.f;//setting the same floor value.
  return *this;
}

class Floor
{
      const string NAME; // name of floor
      const int location; // inches above ground level
      vector<Rider> upRiders; // affected by addNewRider, removeUpRiders,...
      vector<Rider> downRiders; // ...and removeDownRiders functions

    public:
      Floor(const int, const char* const = "unnamed"); // name and location (inches above ground) of floor

      // inline functions
      int getLocation() const {return location;} //returns the current location of the elevator
      //returns if there are riders waiting at the floor
      bool hasRidersWaiting() const {return (upRiders.size()!=0||downRiders.size()!=0)? true : false ;}
      bool hasUpRiders() const{return upRiders.size()!=0 ? true : false ;}
      bool hasDownRiders() const{return downRiders.size()!=0 ? true : false ;}
      int getUpRiderCount() const{return upRiders.size();}
      int getDownRiderCount() const{return downRiders.size();}
      string getName() const{return NAME;}
      // non-inline functions
      bool isPreferredDirectionUp() const; // based on Rider with smallest RiderID
      void addNewRider(const Rider&); // add to up- or down-vector
      vector<Rider> removeUpRiders(int); // int is max #of riders...
      vector<Rider> removeDownRiders(int); //...to move onto elevator

      // just in case you prefer to use "if (...== *toFloor)"
      //  in Elevator::removeRidersForDestinationFloor(), instead of "...== toFloor)"
      bool operator==(const Floor& floor) const {return NAME == floor.NAME;}

      friend ostream& operator<<(ostream&, const Floor&); // say name, location, #'s of up/down riders waiting
};

Floor::Floor(const int a, const char* const s ):NAME(s),location(a){};
void Floor::addNewRider(const Rider& rider) //rider has RiderID and Floor
{
  // if added rider's destination is greater than the floor's location
  if(rider.getDestination().getLocation()>location) //getDestination returns reference of a Floor object, on which we're calling the getLocation function
    upRiders.push_back(rider);// add rider to the upRiders vector
  // else
  else
    downRiders.push_back(rider);// add rider to the downRiders vector
}

vector<Rider> Floor::removeUpRiders(int max) // max = #of unused spaces on elevator, i.i. This many riders can enter the elevator
{ //This function will return the vector of the riders who have been allowed in the Elevator
  // create an empty vector for riders to be removed
  vector<Rider> mveRiders;
  // if there are any up riders...
  if(upRiders.size()!=0)
  {
    // create an empty vector for riders to remain on the floor
    vector<Rider> remRiders;
    // traverse the upRiders vector
    for(int i=0;i<upRiders.size();i++)
    {
      // if there are still spaces left on the elevator...
      if(i<max)
        mveRiders.push_back(upRiders[i]);// add an upRider to the vector of riders to be removed
      // else
      else
        remRiders.push_back(upRiders[i]); // add an upRider to the vector of riders to remain on the floor
    }
    // replace the upRiders vector with the vector of remaining riders
    upRiders=remRiders; //now upRiders contains the remaining riders who could not enter the elevator at this time
  }
  // return the vector of removed riders
  return mveRiders;
}

vector<Rider> Floor::removeDownRiders(int max)
{
  // create an empty vector for riders to be removed
  vector<Rider> mveRiders;
  // if there are any up riders...
  if(downRiders.size()!=0)
  {
    // create an empty vector for riders to remain on the floor
    vector<Rider> remRiders;
    // traverse the upRiders vector
    for(int i=0;i<downRiders.size();i++)
    {
      // if there are still spaces left on the elevator...
      if(i<max)
        mveRiders.push_back(downRiders[i]);// add an upRider to the vector of riders to be removed
      // else
      else
        remRiders.push_back(downRiders[i]); // add an upRider to the vector of riders to remain on the floor
    }
    // replace the upRiders vector with the vector of remaining riders
    downRiders=remRiders;
  }
  // return the vector of removed riders
  return mveRiders;
}

bool Floor::isPreferredDirectionUp() const
{
  // if there are no downRiders, return true
  if(downRiders.size()==0)
    return true;
  // if there are no upRiders, return false
  if(upRiders.size()==0)
    return false;
  // if the ID of the first upRider (upRider[0]) is less than that of the first downRider...
  if(upRiders[0]<downRiders[0])
    return true;// return true
  return false;
  // return false
}
ostream& operator<<(ostream& out, const Floor& f)
{
      out<< f.NAME << "\n";
      out<<abs(f.location)<<" is the location\n";
      out<< f.upRiders.size()<< " Up Riders waiting\n";
      out<< f.downRiders.size()<< " down Riders waiting\n";

      return out;
}
class Elevator
{
      vector<Rider> rider; //Will store the riders inside the elevator

      // class-defined constants
      static int elevatorID; // initialize to zero; use to assign unique ID to each elevator
      static const int IDLE=0; // a unique numeric code
      static const int UP=1; // another unique numeric code
      static const int DOWN=2; // yet another unique numeric code

      // private member variables
      const int ID; // a unique identifier
      const int capacity; // max number of riders, set in constructor
      const int speed; // inches per second, up or down, set in constructor
      const Floor* toFloor; // destination floor, initialized in constructor to zero, Is set to closest floor of the riders
      int location; // inches above ground level, initialized in constructor based on starting floor
      int direction; // equal to IDLE, UP, DOWN, initialized to IDLE in constructor
      bool doorOpen; // initialized to true in constructor

    public:

      Elevator(const int, const int, const Floor&); // capacity, speed, and starting floor

      // inline functions
      bool hasRiders() const{return rider.size()==0 ? false : true ;} // returns false if riders vector is empty
      int getRiderCount() const{return rider.size();} // return size of riders vector
      int getCapacity() const{return capacity;}; // return capacity
      int getAvailableSpace() const{return (capacity-rider.size());} // return capacity minus size of riders vector
      bool isDoorOpen() const {return doorOpen;} // returns value of doorOpen
      bool isIdle() const {return direction==IDLE ? true : false ;} // returns true if direction is IDLE
      bool isDirectionUp() const {return direction==UP ? true : false ;} // returns true if direction is UP
      bool isDirectionDown() const {return direction==DOWN ? true : false ;} // returns true if direction is DOWN
      void closeDoor(){doorOpen=false;} // set doorOpen to false
      void openDoor(){doorOpen=true;} // set doorOpen to true
      void setIdle(){direction=IDLE;} // set direction to IDLE
      void setDirectionUp(){direction=UP;} // set direction to UP
      void setDirectionDown(){direction=DOWN;} // set direction to DOWN
      void moveUp(){location+=speed;} // increment location by #inches per second speed of the elevator
      void moveDown(){location-=speed;} // decrement location by #inches per second speed of the elevator
      void setDestination(const Floor* floor){toFloor=floor;} // set toFloor pointer to floor
      const Floor& getDestination() const {return *toFloor;} // return reference to toFloor
      int getLocation() const{return location;} // return location value
      bool hasADestination() const{return toFloor==0 ? false : true ;} // return false if toFloor is zero

      // non-inline functions
      vector<Rider> removeRidersForDestinationFloor(); // remove riders from vector whose destination is reached
      void addRiders(const vector<Rider>&); // copy riders from parameter vector to riders vector
      void setDestinationBasedOnRiders(); // reset toFloor based on riders' destinations

      bool isNearDestination() const; // true if distance to destination is less than OR EQUAL TO the speed
      void moveToDestinationFloor(); // set location to that of destination floor (if there is one)

      // friend function
      friend ostream& operator<<(ostream&, const Elevator&);
};

int Elevator::elevatorID=0;
Elevator::Elevator(const int c, const int s, const Floor& f):ID(elevatorID),capacity(c),speed(s),toFloor(0)
{
  setIdle();
  location = f.getLocation();
  openDoor();
}
bool Elevator::isNearDestination()const
{
  if(hasADestination())
  {
    if(abs(toFloor->getLocation()-location)<=speed||abs(toFloor->getLocation()-abs(location))<=speed)
      return true;
    else
      return false;
  }
  else
    return false;
}
void Elevator::moveToDestinationFloor()
{
  location=toFloor->getLocation(); //setting location of the elevator to the destination floor.
}
ostream& operator<<(ostream& out, const Elevator& El)
{
  out<< El.rider.size() << " riders in elevator. ";
  out<<"\nElevator's current destination is "<<El.getDestination().getName()<<endl;
  if(El.location>0)
    out<<"Elevator is: "<<abs(El.location)<<" inches above ground, ";
  else if(El.location<0)
    out<<"Elevator is: "<<abs(El.location)<<" inches below ground, ";
  else
    out<<"Ground Floor, ";
  if(El.isDirectionDown())
    out<<"Going Down, ";
  else if(El.isDirectionUp())
    out<<"Going Up, ";
  else
    out<<"Idle, ";
  out<<((El.isDoorOpen())? "Door: open":"Door: closed");
  return out;
}
vector<Rider> Elevator::removeRidersForDestinationFloor() //Destination reached, remove riders
{
  // create empty vector of removed riders to be used as return value
  vector<Rider> removedRiders;
  // if elevator has any riders
  if(rider.size()!=0)
  {
    // create an empty vector for riders who remain on elevator
    vector<Rider> remainingRiders;
    // traverse vector of current riders
    for(int i=0;i<rider.size();i++)
    {
      if(rider[i].getDestination().getLocation() == toFloor->getLocation())// if a rider's destination floor is same as elevator's destination...
        removedRiders.push_back(rider[i]);// add rider to vector of removed riders
      else
        remainingRiders.push_back(rider[i]);  // add rider to vector of remaining riders
    }
    rider=remainingRiders;  // reassign elevator rider vector to vector of remaining riders
  }
  return removedRiders;  // return vector of removed riders
}

void Elevator::addRiders(const vector<Rider>& riders)
{
  // traverse the parameter vector
  for(int i=0;i<riders.size();i++)
  {
    if(rider.size()< capacity)  // if there is still room on the elevator
      rider.push_back(riders[i]);  // add the rider to the elevator's rider vector
  }
}

void Elevator::setDestinationBasedOnRiders() //sets the closest destination
{
  // if there are no riders on the elevator
  if(rider.size()==0)
    return;// exit the function

  // set elevator's destination to the zeroth Rider's destination
  const_cast<const Floor*&>(toFloor) = &rider[0].getDestination();
  // traverse the vector of elevator riders
  for(int i=0;i<rider.size();i++) //rider vector is not sorted in order of closest distances, hence we need to traverse the vector:
  {
    // get the absolute value of the distance from the elevator to the rider's destination floor
    int ridDest = abs(rider[i].getDestination().getLocation()-location);
    // get the absolute value of the distance from the elevator to the elevator's destination floor
    int elDest = abs(toFloor->getLocation()-location);
    // if closer to the rider's destination than to the elevator's destination
    if(ridDest<elDest)//if this rider's destination is closer than previous destination
      const_cast<const Floor*&>(toFloor) = &rider[i].getDestination();// set elevator's destination to this rider's destination
  }
}

class Building
{
      int time;
      vector<Floor*> floor;
      vector<Elevator*> elevator;
    public:
      Building();
      ~Building();
      Building& step(int);
      int getFloorCount() const{return floor.size();} // return #of floors in the vector of Floor*'s
      int getElevatorCount() const{return elevator.size();}// return #of elevators in the vector of Elevator*'s
      const Floor& getFloor(int index) const{return *floor[index];} // return a reference to the "indexth" floor
      const Elevator& getElevator(int index) const{return *elevator[index];} // return a reference to the "indexth" elevator

      friend ostream& operator<<(ostream&, const Building&);
};

void getDifferentFloors(int , int& , int& );

Building::Building()
{
  time=0;

  floor.push_back(new Floor(0,"First floor")); //location in inches, floor name
  floor.push_back(new Floor(100,"Second floor"));
  floor.push_back(new Floor(200,"Third floor"));
  floor.push_back(new Floor(300,"Fourth floor"));
  floor.push_back(new Floor(400,"Fifth floor"));
  elevator.push_back(new Elevator(12,20,*floor[0])); //capacity, speed in inches/sec
  elevator.push_back(new Elevator(10,10,*floor[1]));

}
Building::~Building()
{
  for(int i=0; i<floor.size();i++)
    delete floor[i];
  for(int i=0; i<elevator.size();i++)
    delete elevator[i];
}
ostream& operator<<(ostream& out, const Building& b)
{
  out<<"\n\nTime= "<< b.time << endl;
  for(int i=0; i<(b.elevator).size();i++)
    out<<*(b.elevator[i])<<endl;
  for(int i=0; i<(b.floor).size();i++)
  {
    if((b.floor[i])->hasRidersWaiting())
    out<< *(b.floor[i])<<endl;
  }

  return out;
}
Building& Building::step(int r)//Takes arrival as input
{
  for(int i=0; i< r;i++) //sets a random to and from floor with each arrival
  {
    // create a rider with randomly selected from- and to-floors (where from- and to- are not the same!)
    int a,b;//a represents From-floor and b represents to- floor
    getDifferentFloors(getFloorCount(),a,b); //passing a and b by reference
    // tell the from-floor to add this rider
    floor[a]->addNewRider(Rider(*floor[b]));
  }

  for(int e=0;e<getElevatorCount();e++)
  {
    // if elevator door is closed (move up or down)
    if(!(elevator[e]->isDoorOpen()))
    {
      // if not near enough to destination to reach it in this time step, continue moving
      if(!(elevator[e]->isNearDestination()))
      {
        // if elevator direction is up, move up
        if(elevator[e]->isDirectionUp()) elevator[e]->moveUp();
        // otherwise, move down
        else elevator[e]->moveDown();
      }
      // otherwise it's near enough to destination to reach it in this time step...
      else
      {
        // tell elevator to move to its destination floor
        elevator[e]->moveToDestinationFloor();
        // tell elevator to open its door
        elevator[e]->openDoor();
        // tell elevator to remove riders for its destination floor -- ignore returned vector of removed riders
        elevator[e]->removeRidersForDestinationFloor();
        // get a non-const pointer to the elevator's destination floor
        Floor* f;
        f=const_cast<Floor*>(&(elevator[e]->getDestination()));//f points to the current destination floor

        // if elevator is empty, choose a direction based on longest-waiting rider (the one with the smallest ID) on the floor
        if (elevator[e]->getRiderCount()==0)
        {
          // if the floor's chosen direction is up
          if(f->isPreferredDirectionUp()) elevator[e]->setDirectionUp();
          // tell elevator to set its direction to up
          // otherwise
          else elevator[e]->setDirectionDown();
          // tell elevator to set its direction to down
        }
        // if there is space in the elevator after letting off riders, board new ones
        else
        {
          // if elevator direction is up, board up-riders (if any)...
          if(elevator[e]->isDirectionUp())
            elevator[e]->addRiders(f->removeUpRiders(elevator[e]->getAvailableSpace()));
          // otherwise, board down-riders (if any)
          else
            elevator[e]->addRiders(f->removeDownRiders(elevator[e]->getAvailableSpace()));
        }
        // reassess elevator destination based on its riders
        elevator[e]->setDestinationBasedOnRiders();
      }
    }
    else
    {
      // if elevator has any riders (then they just boarded in the previous time step)
      if(elevator[e]->getRiderCount()>0)
        elevator[e]->closeDoor(); // tell elevator to close its door
      // otherwise
      else
        elevator[e]->setIdle();// tell elevator to go idle
    }
  }


  for(int i=0;i<getFloorCount();i++)
  {
    // if there are no riders waiting on this floor, continue with next floor
    if(!(floor[i]->hasRidersWaiting()))continue;
    // look at each elevator to see if it needs to be sent here
    for(int j=0;j<getElevatorCount();j++)
    {
      // get elevator's relative location (negative if elevator is below floor)
      int loc =elevator[j]->getLocation()-floor[i]->getLocation();
      // if this elevator's idle...
      if(elevator[j]->isIdle())
      {
        // if elevator is above the floor, set elevator direction to down
        if(loc>0)elevator[j]->setDirectionDown();
        // otherwise if it's below, set elevator direction to up
        else elevator[j]->setDirectionUp();
        // set elevator's destination to this floor
        elevator[j]->setDestination(floor[i]);
        // tell elevator to close its door
        elevator[j]->closeDoor();
      }
      else if(floor[i]->hasUpRiders() && elevator[j]->isDirectionUp())
      {
        // get distance from elevator's destination floor to this floor (positive if elevator destination is above this floor)
        int g= (elevator[j])->getDestination().getLocation() - (floor[i])->getLocation();
        // if elevator is below floor and elevator destination is above this floor
        if(g>0 && loc<0)
          elevator[j]->setDestination(floor[i]);// set elevator's destination to this floor
      }
      // else if there are riders on this floor waiting to go down, and the elevator is going down...
      else if(floor[i]->hasDownRiders() && elevator[j]->isDirectionDown())
      {
        // get distance from elevator's destination floor to this floor (negative if elevator destination is below this floor)
        int g= (elevator[j]->getDestination()).getLocation() - (floor[i])->getLocation();
        // if elevator is above floor and elevator destination is below this floor [10]
        if(g<0 && loc>0)
          elevator[j]->setDestination(floor[i]);// set elevator's destination to this floor
      }
    }
  }
  time++;
  return *this;
}
void getDifferentFloors(int max, int& a, int& b)
{
    //max is the num of floors
  do
  {
    a = rand() % max; // range is 0 to (max-1)
    b = rand() % max; // range is 0 to (max-1)
  } while (a == b); // try again if they are the same
}





int getArrivalsForThisSecond()
{
  int arrivals = 0;
  arrivals=rand()%3;
  return arrivals;
}


int main()
{
  cout << "ElevatorSystemSimulation.cpp\n";
  cout<<"\nCS524 Mini Project.\n";
  Building building;


  // continue the simulation for 600 more seconds
  for (int i = 0;i<=601; i++)
  {
    cout << building.step(getArrivalsForThisSecond()) << endl;
    if (!(i % 20)) // pause every 20 seconds
    {
      cout << "Press ENTER to continue, X-ENTER to quit...\n";
      if (cin.get() > 31) break;
    }
  }

}
