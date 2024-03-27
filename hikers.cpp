#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <yaml-cpp/yaml.h>

struct Hiker
{
    std::string name;
    double pace; // feet per minute
};

struct Bridge
{
    int id;
    double length; // feet
    std::vector<Hiker> hikers; 
};

struct Result
{
    double total_t; // minutes
    std::vector<double> bridge_t;
};


/**
 * Sorts in descending order. 
*/
void sort_hikers_by_pace(std::vector<Hiker> &hikers)
{
    std::sort(hikers.begin(), hikers.end(), [](const Hiker &a, const Hiker &b)
              { return a.pace > b.pace; });
}

std::vector<Bridge> parse_bridges(std::string bridge_file)
{
    std::ifstream fin(bridge_file);
    YAML::Node doc = YAML::Load(fin);

    std::vector<Bridge> bridges;
    for (const auto &bridgeNode : doc["bridge"])
    {
        Bridge bridge;
        bridge.id = bridgeNode["id"].as<int>();
        bridge.length = bridgeNode["length"].as<int>();

        for (const auto &hikerNode : bridgeNode["hikers"])
        {
            Hiker hiker;
            hiker.name = hikerNode["name"].as<std::string>();
            hiker.pace = hikerNode["pace"].as<double>();
            bridge.hikers.push_back(hiker);
        }

        bridges.push_back(bridge);
    }
    return bridges;
}

void print_hikers(std::vector<Hiker> hikers)
{
    for (const auto &hiker : hikers)
    {
        std::cout << "  Name: " << hiker.name << ", Pace: " << hiker.pace << " feet per minute" << std::endl;
    }
    std::cout << std::endl;
}

void print_bridges(std::vector<Bridge> bridges)
{
    std::cout << "*** Bridge input data ***\n";
    for (const auto &bridge : bridges)
    {
        std::cout << "Bridge ID: " << bridge.id << std::endl;
        std::cout << "Length: " << bridge.length << " feet" << std::endl;
        std::cout << "Hikers:" << std::endl;
        print_hikers(bridge.hikers);
    }
}


/**
 * Gets the fastest hiker, i.e., one with highest pace, from a vector of hikers.
 * Not used.
 */
// Hiker get_fastest_hiker(const std::vector<Hiker> &crew)
// {
//     if (crew.empty())
//     {
//         std::cout << "No hikers. Returning a default hiker." << std::endl;
//         return Hiker{"", 0.0};
//     }
//     Hiker fastest_hiker = Hiker{"", 0.0};
//     for (const auto &hiker : crew)
//     {
//         if (hiker.pace > fastest_hiker.pace)
//         {
//             fastest_hiker = hiker;
//         }
//     }
//     return fastest_hiker;
// }

/**
 * Concatenates two hiker vectors. Second argument is added to the first.
 *
 * @param crew The vector being modified.
 * @param newbies The vector being added to crew.
 */
void add_hikers(std::vector<Hiker> &crew, const std::vector<Hiker> &newbies)
{
    crew.insert(
        crew.end(),
        std::make_move_iterator(newbies.begin()),
        std::make_move_iterator(newbies.end()));
}

/**
 * Simulates optimal bridge crossing. 
 * 
 * logic behind the process is as follows. 
 * 
 * The optimal solution is to have the fastest person cross the bridge each time, 
 *  ferrying each slower person across with them. 
 * 
 * This strategy minimizes the overall waiting time required for the torch to return
 *   to the contingent of hikers who have not crossed the bridge yet.
 *
 * @param bridges vector of bridges, including hikers at each bridge, parsed from yml file.  
 * @param result Result struct, holds the time taken to cross each bridge and the total time to cross them all. 
 */
void simulate_optimal_crossing(const std::vector<Bridge> bridges, Result &result){
    std::cout << "*** Beginning to cross bridges ***\n"; 
    double total_time = 0.0;
    std::vector<Hiker> present_hikers;
    for (const auto &bridge : bridges)
    {
        std::cout << "Bridge length : " << bridge.length << std::endl;
        // std::vector<Hiker> crossed;
        add_hikers(present_hikers, bridge.hikers);

        sort_hikers_by_pace(present_hikers);
        std::vector<Hiker> not_crossed = present_hikers; // create a copy of all hikers.
        // Hiker fastest_hiker = get_fastest_hiker(present_hikers); // Could be faster to compare fastest of new hikers with fastest of previous group.

        double bridge_time = 0.0;
        Hiker fastest_hiker = not_crossed.front();
        std::cout << "\tThe fastest hiker is " << fastest_hiker.name << " with a pace of " << fastest_hiker.pace << std::endl;
        do
        {
            // Slowest hiker crosses the bridge with the fastest hiker.
            Hiker ferried_hiker = not_crossed.back();
            not_crossed.pop_back();
            bridge_time += (bridge.length / ferried_hiker.pace);
            // add_hikers(crossed, std::vector<Hiker> {ferried_hiker});
            
            
            std::cout << "\tHiker " << ferried_hiker.name << " crossed (pace : " << ferried_hiker.pace << "). Total Time on bridge " << bridge_time << std::endl;

            if (not_crossed.size() > 1)
            { // Fastest hiker runs back across the bridge.
                bridge_time += (bridge.length / fastest_hiker.pace);
                std::cout << "\t\tHiker " << fastest_hiker.name << " crossed back. Total Time on bridge " << bridge_time << std::endl;
            }
        } while (not_crossed.size() > 1);
        result.bridge_t[bridge.id] = bridge_time;
        total_time += bridge_time;
        std::cout << "Total time: " << total_time << std::endl;
    }
    result.total_t = total_time;
}

void print_result(const Result& result) {
    std::cout << "\n*** Result Summary ***" << std::endl;    
    std::cout << "Bridge times:" << std::endl;
    for (size_t i = 0; i < result.bridge_t.size(); ++i) {
        std::cout << "\tBridge " << i << ": " << result.bridge_t[i] << " minutes" << std::endl;
    }
    std::cout << "Total time: " << result.total_t << " minutes" << std::endl;
}

int main(int argc, char* argv[]) 
{
    std::string input_file;
    if (argc != 2) {
        std::cout << "using default file, hikers.yml\n"; 
        input_file = "hikers.yml";
    }
    else{
        input_file = argv[1];
    }
    // Load the YAML file
    bool DEBUG = false;
    double crossing_time;

    std::vector<Bridge> bridges = parse_bridges(input_file);
    std::vector<double> bridge_t(bridges.size());
    Result result {0.0, bridge_t};
    
    print_bridges(bridges);    

    simulate_optimal_crossing(bridges, result);
    
    print_result(result);

    return 0;
}
