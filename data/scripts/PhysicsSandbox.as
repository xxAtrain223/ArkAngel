class PhysicsSandbox
{

    PhysicsSandbox()
    {
        loadB2World("data/b2worlds/truck.json");
    }

    void update()
    {
        print("Updated PhysicsSandbox object");
    }
}