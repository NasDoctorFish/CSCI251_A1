class Employee
{
public:
    Employee(const char *, const char *);
    virtual void print() const;
    ~Employee();

private:
    char *firstName;
    char *lastName;
}

// can use final on function
class Circle : public shape
{
 public : virtual void draw() final // stop method overriding
    {
        cout << "" << endl;
    }