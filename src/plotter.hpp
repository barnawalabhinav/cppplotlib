#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <assert.h>
#include <fstream>
#include <unistd.h>

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
        else
            std::cerr << "Could not set up pipe with gnuplot" << std::endl;
    }

    inline virtual ~Plotter()
    {
        if (gnuplotPipe)
        {
            pclose(gnuplotPipe);

            for (int i = 0; i < cnt_files; i++)
            {
                unlink((std::to_string(i) + ".dat").c_str());
                // std::remove((std::to_string(i) + ".dat").c_str());
            }
        }
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

        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);

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
        cnt_files++;
    }

    template <typename T1, typename T2>
    inline void createPlot(const std::vector<T1> &x, const std::vector<T2> &y, const char *line_title = "", const char *linecolor = "auto", const T1 shift = static_cast<T1>(0))
    {
        fprintf(gnuplotPipe, "plot ");
        std::string filename = std::to_string(cnt_files) + ".dat";
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

        cnt_files++;
    }

    template <typename T2>
    inline void createPlot(const std::vector<T2> &y, const char *line_title = "", const char *linecolor = "auto")
    {
        fprintf(gnuplotPipe, "plot ");
        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);

        for (int i = 0; i < y.size(); i++)
            fout << i << " " << y[i] << "\n";

        if (linecolor == "auto")
            fprintf(gnuplotPipe, "\"%s\" using 1:2 smooth unique with lines title '%s'", filename.c_str(), line_title);
        else
            fprintf(gnuplotPipe, "\"%s\" using 1:2 smooth unique with lines linecolor '%s' title '%s'", filename.c_str(), linecolor, line_title);

        cnt_files++;
    }

    template <typename T1, typename T2>
    inline void addPlot(const std::vector<T1> &x, const std::vector<T2> &y, const char *line_title, const char *linecolor = "auto", const T1 shift = static_cast<T1>(0))
    {
        std::string filename = std::to_string(cnt_files) + ".dat";
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

        cnt_files++;
    }

    template <typename T2>
    inline void addPlot(const std::vector<T2> &y, const char *line_title, const char *linecolor = "auto")
    {
        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);

        for (int i = 0; i < y.size(); i++)
            fout << i << " " << y[i] << "\n";

        if (linecolor == "auto")
            fprintf(gnuplotPipe, ", \"%s\" using 1:2 smooth unique with lines title '%s'", filename.c_str(), line_title);
        else
            fprintf(gnuplotPipe, ", \"%s\" using 1:2 smooth unique with lines linecolor '%s' title '%s'", filename.c_str(), linecolor, line_title);

        cnt_files++;
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

    template <typename T>
    void set_xtics(const std::vector<T> &x)
    {
        std::string xtics_cmd = "set xtics (";
        for (int i = 0; i < x.size() - 1; i++)
            xtics_cmd += "\"" + x[i] + "\" " + std::to_string(i) + ", ";

        xtics_cmd += "\"" + x.back() + "\" " + std::to_string(x.size() - 1) + ")\n";
        fprintf(gnuplotPipe, "%s", xtics_cmd.c_str());
    }

    template <typename T>
    void set_ytics(const std::vector<T> &y)
    {
        std::string ytics_cmd = "set ytics (";
        for (int i = 0; i < y.size() - 1; i++)
            ytics_cmd += "\"" + y[i] + "\" " + std::to_string(i) + ", ";

        ytics_cmd += "\"" + y.back() + "\" " + std::to_string(y.size() - 1) + ")\n";
        fprintf(gnuplotPipe, "%s", ytics_cmd.c_str());
    }

private:
    FILE *gnuplotPipe;
    int cnt_files = 0;
};
