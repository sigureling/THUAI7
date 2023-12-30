#include "AI.h"
#include "logic.h"
#include "structures.h"
#include <tclap/CmdLine.h>
#include <array>
#include <string_view>

#undef GetMessage
#undef SendMessage
#undef PeekMessage

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

using namespace std::literals::string_view_literals;

// Generated by http://www.network-science.de/ascii/ with font "standard"
static constexpr std::string_view welcomeString = R"welcome(
            ______________ ___  ____ ___  _____  .____________ 
            \__    ___/   |   \|    |   \/  _  \ |   \______  \
              |    | /    ~    \    |   /  /_\  \|   |   /    /
              |    | \    Y    /    |  /    |    \   |  /    / 
              |____|  \___|_  /|______/\____|__  /___| /____/  
                            \/                 \/ 
            _________ __                  __      __               
            /   _____//  |______ _______  /  \    /  \_____ _______ 
            \_____  \\   __\__  \\_  __ \ \   \/\/   /\__  \\_  __ \
            /        \|  |  / __ \|  | \/  \        /  / __ \|  | \/
          /_______  /|__| (____  /__|      \__/\  /  (____  /__|   
                  \/           \/               \/        \/    
)welcome"sv;

int THUAI7Main(int argc, char** argv, CreateAIFunc AIBuilder)
{
    int pID = 0;
    int tID = 0;
    std::string sIP = "172.22.32.1";
    std::string sPort = "8888";
    bool file = false;
    bool print = false;
    bool warnOnly = false;
    extern const std::array<THUAI7::ShipType, 4> shipTypeDict;
    // {
    //     file = true;
    //     print = true;
    //     Logic logic(playerType, pID, trickerType, studentType);
    //     logic.Main(AIBuilder, sIP, sPort, file, print, warnOnly);
    //     return 0;
    // }

    // 使用cmdline的正式版本
    try
    {
        TCLAP::CmdLine cmd("THUAI7 C++ interface commandline parameter introduction");

        TCLAP::ValueArg<std::string> serverIP("I", "serverIP", "Server`s IP 127.0.0.1 in default", false, "127.0.0.1", "string");
        cmd.add(serverIP);

        TCLAP::ValueArg<std::string> serverPort("P", "serverPort", "Port the server listens to 7777 in default", false, "7777", "USORT");
        cmd.add(serverPort);

        std::vector<int> validTeamIDs{0, 1};  // 红0蓝1
        TCLAP::ValuesConstraint<int> teamIdConstraint(validTeamIDs);
        TCLAP::ValueArg<int> teamID("t", "teamID", "Team ID 0,1 valid only", true, -1, &teamIdConstraint);
        cmd.add(teamID);

        std::vector<int> validPlayerIDs{0, 1, 2, 3, 4};
        TCLAP::ValuesConstraint<int> playerIdConstraint(validPlayerIDs);
        TCLAP::ValueArg<int> playerID("p", "playerID", "Player ID 0,1,2,3,4 valid only", true, -1, &playerIdConstraint);
        cmd.add(playerID);

        std::string DebugDesc = "Set this flag to save the debug log to ./logs folder.\n";
        TCLAP::SwitchArg debug("d", "debug", DebugDesc);
        cmd.add(debug);

        std::string OutputDesc = "Set this flag to print the debug log to the screen.\n";
        TCLAP::SwitchArg output("o", "output", OutputDesc);
        cmd.add(output);

        TCLAP::SwitchArg warning("w", "warning", "Set this flag to only print warning on the screen.\n"
                                                 "This flag will be ignored if the output flag is not set\n");
        cmd.add(warning);

        cmd.parse(argc, argv);
        tID = teamID.getValue();
        pID = playerID.getValue();
        sIP = serverIP.getValue();
        sPort = serverPort.getValue();

        file = debug.getValue();
        print = output.getValue();
        if (print)
            warnOnly = warning.getValue();
    }
    catch (TCLAP::ArgException& e)
    {
        std::cerr << "Parsing error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
    try
    {
        THUAI7::PlayerType playerType;
        THUAI7::ShipType shipType = THUAI7::ShipType::NullShipType;
        if (pID == 0)
            playerType = THUAI7::PlayerType::Team;
        else
        {
            playerType = THUAI7::PlayerType::Ship;
            shipType = shipTypeDict[pID];
        }
#ifdef _MSC_VER
        std::cout
            << welcomeString << std::endl;
#endif
        Logic logic(pID, tID, playerType, shipType);
        logic.Main(AIBuilder, sIP, sPort, file, print, warnOnly);
    }
    catch (const std::exception& e)
    {
        std::cerr << "C++ Exception: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown Exception\n";
    }
    return 0;
}

std::unique_ptr<IAI> CreateAI(int64_t pID)
{
    return std::make_unique<AI>(pID);
}

int main(int argc, char* argv[])
{
    return THUAI7Main(argc, argv, CreateAI);
}