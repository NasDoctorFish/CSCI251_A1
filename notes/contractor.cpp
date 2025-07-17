int main()
{
    Contractor cn("Bob", "Smith", 40.0, 10.00);

    // use derived class reference
    Contractor &r1 = cn;
    r1.print(); // Contractor print()

    // use base class reference
    Employee &r2 = cn;
    r2.print(); // which print?? => Employee print()
    // r2가 cn의 reference varaibel이기는 한데 Employee로 정의되어서 .print()는 Employee꺼 씀.

    // Use derived class pointer
    Contractor *p1 = &cn;
    p1->print(); // contractor print()

    // use base class pointer
    Employee *p2 = &cn;
    p2->print(); // employee print()

    // casting base-class pointer to derived-class pointer
    p1 = static_cast<Contractor *>(p2); // cast
    p1->print(); // contractor print()
    return 0;
}
