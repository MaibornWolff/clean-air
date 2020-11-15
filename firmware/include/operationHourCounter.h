// Operating Check time
#define UPDATE_OPERATING_HOURS_INTERVAL 3600000

class OperationHourCounter
{
private:
    void loadHours();
    void updateHours();

public:
    // Constructor.
    void setup(JsonHandler);

    // Checks if the given interval has passed and then starts to check for new software.
    void countOperationHours();
};
