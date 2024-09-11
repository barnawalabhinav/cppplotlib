#include "src/plotter.hpp"

using namespace std;

int main()
{
    std::vector<int> x = {0, 2, 3, 4, 1};
    std::vector<float> y = {0.2, 0.3, 0.1, 0.9, 0.75};
    std::vector<std::string> xt = {"1", "6", "#", "2", "$"};

    Plotter plt(1200, 900, 20, false);
    plt.set_savePath("plot.png");
    plt.set_logscale_y();
    plt.unset_logscale_y();
    plt.set_xlim(0 - 0.1, 4 + 0.1);
    plt.set_ylim(0 - 0.1, 6 + 0.2);
    plt.xticks(xt);
    plt.createPlot(y, "1", "auto", Plotter::None, 1.0, 5.0, Plotter::DASH_N_DOT);
    std::vector<double> ub(y.size()), lb(y.size());
    for (int i = 0; i < y.size(); i++) {
        ub[i] = y[i] + 0.2;
        lb[i] = y[i] - 0.1;
    }
    plt.fillBetween(ub, lb);
    plt.addPlot(x, "2", "blue", Plotter::None, 5.0, Plotter::POINT);
    plt.addPlot(xt, "3", "green", Plotter::None, 5.0, Plotter::DASH_N_DOUBLE_DOT);
    plt.plot();

    plt.reset();

    plt.set_savePath("scatter.png");
    plt.set_xlim(0 - 0.1, 4 + 0.1);
    plt.set_ylim(0 - 0.1, 6 + 0.2);
    plt.xticks(xt);
    plt.createScatterPlot(y, "x", 0.5, "1", "red");
    plt.addPlot(x, "2", "grey", Plotter::None, 5.0);
    plt.addScatterPlot(xt, "s", 2.0, "3", "blue");
    plt.plot();
    
    x = {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5};
    y = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    std::vector<double> z = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};

    x = {-3, -3, -3, -2, -2, -2, -1, -1, -1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3};
    y = {1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3};
    z = {9, 9, 9, 4, 4, 4, 1, 1, 1, 0, 0, 0, 1, 1, 1, 4, 4, 4, 9, 9, 9};

    // x = {0, 0, 0, 1, 1, 1, 2, 2, 2};
    // y = {0, 1, 2, 0, 1, 2, 0, 1, 2};
    // z = {0, 1, 4, 1, 2, 5, 4, 5, 8};
    
    plt.reset();
    plt.set_savePath("3dplot.png");
    plt.show_grid(true);
    // plt.set_xlim(0, 5);
    // plt.set_ylim(0, 5);
    // plt.set_zlim(0, 5);
    plt.set_xlabel("X");
    plt.set_ylabel("Y");
    plt.set_zlabel("Z");
    plt.createLinePlot3D(x, y, z, "1", "blue", Plotter::BoxF, 2.0, 2.0, Plotter::DASH_N_DOT);
    // plt.plotSurface(x, y, z);
    plt.plot();

    return 0;
}