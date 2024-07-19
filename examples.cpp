#include "src/plotter.hpp"

using namespace std;

int main()
{
    std::vector<int> x = {0, 2, 3};
    std::vector<float> y = {0.2, 0.3, 0.1, 0.9, 0.75};

    Plotter plt;
    plt.set_savePath("plot.png");
    // plt.set_xlim(-1, 4);
    // plt.set_ylim(0, 0.5);
    std::vector<std::string> xt = {"1", "6", "#", "2", "$"};
    // plt.xticks(xt);
    // plt.yticks(xt);
    plt.createHistogram(x, y);
    plt.plot();

    plt.reset();
    
    return 0;
}