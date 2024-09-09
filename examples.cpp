#include "src/plotter.hpp"

using namespace std;

int main()
{
    std::vector<int> x = {0, 2, 3, 4, 1};
    std::vector<float> y = {0.2, 0.3, 0.1, 0.9, 0.75};
    std::vector<std::string> xt = {"1", "6", "#", "2", "$"};

    Plotter plt(1200, 900, 20, false);
    plt.set_savePath("plot.png");
    plt.set_xlim(0 - 0.1, 4 + 0.1);
    plt.set_ylim(0 - 0.1, 6 + 0.2);
    plt.xticks(xt);
    plt.createPlot(y, "1", "auto", Plotter::None, 1.0, 5.0, Plotter::DASH_N_DOT);
    plt.addPlot(x, "2", "blue", "", 5.0, Plotter::POINT);
    plt.addPlot(xt, "3", "green", "", 5.0, Plotter::DASH_N_DOUBLE_DOT);
    plt.plot();

    plt.reset();

    plt.set_savePath("scatter.png");
    plt.set_xlim(0 - 0.1, 4 + 0.1);
    plt.set_ylim(0 - 0.1, 6 + 0.2);
    plt.xticks(xt);
    plt.createScatterPlot(y, "x", 0.5, "1", "red");
    plt.addPlot(x, "2", "grey", "", 5.0);
    plt.addScatterPlot(xt, "s", 2.0, "3", "blue");
    plt.plot();
    

    return 0;
}