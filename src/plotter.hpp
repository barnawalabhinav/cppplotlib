#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <assert.h>
#include <fstream>

class Plotter
{
public:
    inline Plotter(int size_x = 1200, int size_y = 900, int fontSize = 20, bool debugMode = false)
    {
        if (debugMode)
        {
            std::string command = (std::string) "cat > " + "debug_plotter.txt";
            gnuplotPipe = popen(command.c_str(), "w");
        }
        else
            gnuplotPipe = popen("gnuplot -persistent", "w");

        if (gnuplotPipe)
            fprintf(gnuplotPipe, "set terminal pngcairo enhanced font ',%d' size %d, %d\n", fontSize, size_x, size_y);
    }

    inline virtual ~Plotter()
    {
        if (gnuplotPipe)
            pclose(gnuplotPipe);

        // for (int i = 0; i < cnt_lines; i++)
        //     std::remove((std::to_string(i) + ".dat").c_str());
    }

    inline void reset_plot(int size_x = 1200, int size_y = 900, int fontSize = 20)
    {
        fprintf(gnuplotPipe, "set terminal pngcairo enhanced font ',%d' size %d, %d\n", fontSize, size_x, size_y);
    }

    inline void plot()
    {
        if (gnuplotPipe)
        {
            fprintf(gnuplotPipe, "\n");
            fprintf(gnuplotPipe, "unset multiplot\n");
        }
    }

    inline void set_multiplot(int multi_layout_x = 3, int multi_layout_y = 4, const char *title = "")
    {
        fprintf(gnuplotPipe, "set multiplot layout %d, %d title '%s'\n", multi_layout_x, multi_layout_y, title);
    }

    inline void unset_multiplot()
    {
        fprintf(gnuplotPipe, "unset multiplot\n");
    }

    inline void createBoxPlot(const std::vector<std::string> &x, const std::vector<std::vector<double>> &y, const bool show_xticks = true, const double box_width = 0.5, const char *color = "auto")
    {
        fprintf(gnuplotPipe, "set style data boxplot\n");
        fprintf(gnuplotPipe, "set style boxplot outliers pointtype 7\n");

        std::string filename = std::to_string(cnt_lines) + "box.dat";
        std::ofstream fout(filename);

        std::string xtics_cmd = "set xtics (";
        for (int i = 0; i < x.size() - 1; i++)
            xtics_cmd += "\"" + x[i] + "\" " + std::to_string(i + 1) + ", ";

        xtics_cmd += "\"" + x.back() + "\" " + std::to_string(x.size()) + ")\n";

        if (show_xticks)
            fprintf(gnuplotPipe, "%s", xtics_cmd.c_str());

        if (color != "auto")
            for (int i = 0; i < y.size(); i++)
                fprintf(gnuplotPipe, "set linetype %d lc '%s' lw 2\n", i + 1, color);

        for (int j = 0; j < y[0].size() - 1; j++)
        {
            for (int i = 0; i < y.size() - 1; i++)
                fout << y[i][j] << " ";
            fout << y.back()[j] << "\n";
        }

        fprintf(gnuplotPipe, "plot '%s' using (1):1 title '' with boxplot,", filename.c_str());
        for (int i = 1; i < y.size(); i++)
            fprintf(gnuplotPipe, "'' using (%d):%d title '' with boxplot,", i + 1, i + 1);

        fprintf(gnuplotPipe, "\n");
        fprintf(gnuplotPipe, "unset style boxplot\n");
        cnt_lines++;
    }

    inline void createPlot(const std::vector<double> &x, const std::vector<double> &y, const char *line_title = "", const char *linecolor = "auto", const int shift = 0.0)
    {
        if (x.size() <= 1 || y.size() <= 1)
            return;

        fprintf(gnuplotPipe, "plot ");
        std::string filename = std::to_string(cnt_lines) + ".dat";
        std::ofstream fout(filename);

        for (int i = 0; i < x.size(); i++)
        {
            if (i >= y.size())
                break;
            fout << x[i] + shift << " " << y[i] << "\n";
        }
        if (linecolor == "auto")
            fprintf(gnuplotPipe, "\"%s\" using 1:2 smooth unique with lines title '%s'", filename.c_str(), line_title);
        else
            fprintf(gnuplotPipe, "\"%s\" using 1:2 smooth unique with lines linecolor '%s' title '%s'", filename.c_str(), linecolor, line_title);

        cnt_lines++;
    }

    inline void addPlot(const std::vector<double> &x, const std::vector<double> &y, const char *line_title, const char *linecolor = "auto", int shift = 0.0)
    {
        if (x.size() <= 1 || y.size() <= 1)
            return;

        std::string filename = std::to_string(cnt_lines) + ".dat";
        std::ofstream fout(filename);

        for (int i = 0; i < x.size(); i++)
        {
            if (i >= y.size())
                break;
            fout << x[i] + shift << " " << y[i] << "\n";
        }
        if (linecolor == "auto")
            fprintf(gnuplotPipe, ", \"%s\" using 1:2 smooth unique with lines title '%s'", filename.c_str(), line_title);
        else
            fprintf(gnuplotPipe, ", \"%s\" using 1:2 smooth unique with lines linecolor '%s' title '%s'", filename.c_str(), linecolor, line_title);

        cnt_lines++;
    }

    void set_xlabel(const char *label)
    {
        if (gnuplotPipe)
            fprintf(gnuplotPipe, "\nset xlabel '%s'\n", label);
    }

    void set_ylabel(const char *label)
    {
        if (gnuplotPipe)
            fprintf(gnuplotPipe, "\nset ylabel '%s'\n", label);
    }

    void set_title(const char *title)
    {
        if (gnuplotPipe)
            fprintf(gnuplotPipe, "\nset title '%s'\n", title);
    }

    void set_savePath(const char *savePath)
    {
        if (gnuplotPipe)
            fprintf(gnuplotPipe, "\nset output '%s'\n", savePath);
    }

private:
    FILE *gnuplotPipe;
    int cnt_lines = 0;
};
