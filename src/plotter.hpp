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
            gnuplotPipe = fopen("debug_plotter.txt", "w");
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

    inline void reset(int size_x = 1200, int size_y = 900, int fontSize = 20)
    {
        fprintf(gnuplotPipe, "reset\n");
        fprintf(gnuplotPipe, "set terminal pngcairo enhanced font ',%d' size %d, %d\n", fontSize, size_x, size_y);
    }

    inline void plot()
    {
        if (gnuplotPipe)
            fprintf(gnuplotPipe, "\n");
    }

    inline void set_multiplot(int multi_layout_x = 3, int multi_layout_y = 4, const char *title = "")
    {
        fprintf(gnuplotPipe, "set multiplot layout %d, %d title '%s'\n", multi_layout_x, multi_layout_y, title);
    }

    inline void unset_multiplot()
    {
        fprintf(gnuplotPipe, "unset multiplot\n");
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

    inline void set_grid(bool show_grid = true)
    {
        if (gnuplotPipe)
        {
            if (show_grid)
                fprintf(gnuplotPipe, "set grid\n");
            else
                fprintf(gnuplotPipe, "unset grid\n");
        }
    }

    inline void set_legend(const char *position = "right")
    {
        if (gnuplotPipe)
            fprintf(gnuplotPipe, "set key %s\n", position);
    }

    inline void unset_legend()
    {
        if (gnuplotPipe)
            fprintf(gnuplotPipe, "unset key\n");
    }

    inline void set_xlim(double min, double max)
    {
        if (gnuplotPipe)
            fprintf(gnuplotPipe, "set xrange [%f:%f]\n", min, max);
    }

    inline void set_ylim(double min, double max)
    {
        if (gnuplotPipe)
            fprintf(gnuplotPipe, "set yrange [%f:%f]\n", min, max);
    }

    template <typename T2>
    void xticks(const std::vector<T2> &ticks)
    {
        std::string xtics_cmd = "set xtics (";
        for (int i = 0; i < ticks.size() - 1; i++)
            xtics_cmd += "\"" + ticks[i] + "\" " + std::to_string(i) + ", ";

        xtics_cmd += "\"" + ticks.back() + "\" " + std::to_string(ticks.size() - 1) + ")\n";
        fprintf(gnuplotPipe, "%s", xtics_cmd.c_str());
    }

    template <typename T1, typename T2>
    void xticks(const std::vector<T1> &x, const std::vector<T2> &ticks)
    {
        if (x.size() != ticks.size())
            throw std::runtime_error("ERROR: xticks size doesn't match with x!");

        std::string xtics_cmd = "set xtics (";
        for (int i = 0; i < x.size() - 1; i++)
            xtics_cmd += "\"" + ticks[i] + "\" " + std::to_string(x[i]) + ", ";

        xtics_cmd += "\"" + ticks.back() + "\" " + std::to_string(x.back()) + ")\n";
        fprintf(gnuplotPipe, "%s", xtics_cmd.c_str());
    }

    template <typename T2>
    void yticks(const std::vector<T2> &ticks)
    {
        std::string ytics_cmd = "set ytics (";
        for (int i = 0; i < ticks.size() - 1; i++)
            ytics_cmd += "\"" + ticks[i] + "\" " + std::to_string(i) + ", ";

        ytics_cmd += "\"" + ticks.back() + "\" " + std::to_string(ticks.size() - 1) + ")\n";
        fprintf(gnuplotPipe, "%s", ytics_cmd.c_str());
    }

    template <typename T1, typename T2>
    void yticks(const std::vector<T1> &y, const std::vector<T2> &ticks)
    {
        if (y.size() != ticks.size())
            throw std::runtime_error("ERROR: yticks size doesn't match with y!");

        std::string ytics_cmd = "set ytics (";
        for (int i = 0; i < y.size() - 1; i++)
            ytics_cmd += "\"" + ticks[i] + "\" " + std::to_string(y[i]) + ", ";

        ytics_cmd += "\"" + ticks.back() + "\" " + std::to_string(y.back()) + ")\n";
        fprintf(gnuplotPipe, "%s", ytics_cmd.c_str());
    }

    inline void createBoxPlot(const std::vector<std::string> &x, const std::vector<std::vector<double>> &y, const bool show_xticks = true, const double box_width = 0.5, const char *color = "auto")
    {
        fprintf(gnuplotPipe, "set style data boxplot\n");
        fprintf(gnuplotPipe, "set style boxplot outliers pointtype 7\n");

        if (color != "auto")
            for (int i = 0; i < y.size(); i++)
                fprintf(gnuplotPipe, "set linetype %d lc '%s' lw 2\n", i + 1, color);

        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);
        for (int j = 0; j < y[0].size() - 1; j++)
        {
            for (int i = 0; i < y.size() - 1; i++)
                fout << y[i][j] << " ";
            fout << y.back()[j] << "\n";
        }
        fout.close();

        fprintf(gnuplotPipe, "plot '%s' using (1):1 title '' with boxplot,", filename.c_str());
        for (int i = 1; i < y.size(); i++)
            fprintf(gnuplotPipe, "'' using (%d):%d title '' with boxplot,", i + 1, i + 1);

        fprintf(gnuplotPipe, "\n");
        fprintf(gnuplotPipe, "unset style boxplot\n");
        cnt_files++;
    }

    template <typename T1, typename T2>
    inline void createScatterPlot(const std::vector<T1> &x, const std::vector<T2> &y, const char *pointtype = "O", const double pointsize = 1.0, const char *title = "", const char *point_color = "auto", const bool set_range = true)
    {
        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);
        for (int i = 0; i < x.size(); i++)
        {
            if (i >= y.size())
                break;
            fout << x[i] << " " << y[i] << "\n";
        }
        fout.close();

        if (set_range)
        {
            fprintf(gnuplotPipe, "stats '%s' using 1:2 nooutput\n", filename.c_str());
            fprintf(gnuplotPipe, "x_offset = (STATS_max_x - STATS_min_x) * 0.05\n");
            fprintf(gnuplotPipe, "y_offset = (STATS_max_y - STATS_min_y) * 0.05\n");
            fprintf(gnuplotPipe, "set xrange [STATS_min_x - x_offset:STATS_max_x + x_offset]\n");
            fprintf(gnuplotPipe, "set yrange [STATS_min_y - y_offset:STATS_max_y + y_offset]\n");
        }

        fprintf(gnuplotPipe, "plot ");
        if (point_color == "auto")
            fprintf(gnuplotPipe, "\"%s\" using 1:2 with points pointtype '%s' pointsize %f title '%s'", filename.c_str(), pointtype, pointsize, title);
        else
            fprintf(gnuplotPipe, "\"%s\" using 1:2 with points pointtype '%s' pointsize %f linecolor '%s' title '%s'", filename.c_str(), pointtype, pointsize, point_color, title);

        cnt_files++;
    }

    template <typename T2>
    inline void createHistogram(const std::vector<T2> &y, const double bin_width = 0.0, const char *color = "auto", const char *title = "", const double opacity = 1.0)
    {
        fprintf(gnuplotPipe, "set style data histograms\n");

        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);
        for (int i = 0; i < y.size(); i++)
            fout << i << " " << y[i] << "\n";
        fout.close();

        // Determine the range of the data
        fprintf(gnuplotPipe, "stats '%s' using 1:2 nooutput\n", filename.c_str());
        fprintf(gnuplotPipe, "y_offset = (STATS_max_y - STATS_min_y) * 0.05\n");
        fprintf(gnuplotPipe, "set yrange [STATS_min_y - y_offset:STATS_max_y + y_offset]\n");

        fprintf(gnuplotPipe, "set boxwidth %f relative\n", bin_width);
        fprintf(gnuplotPipe, "set style fill solid %f\n", opacity);

        if (std::string(color) == "auto")
            fprintf(gnuplotPipe, "plot '%s' using 2 title '%s'\n", filename.c_str(), title);
        else
            fprintf(gnuplotPipe, "plot '%s' using 2 linecolor '%s' title '%s'\n", filename.c_str(), color, title);

        cnt_files++;
    }

    template <typename T1, typename T2>
    inline void createHistogram(const std::vector<T1> &x, const std::vector<T2> &y, const double bin_width = 0.0, const char *color = "auto", const char *title = "", const double opacity = 1.0)
    {
        fprintf(gnuplotPipe, "set style data histograms\n");

        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);
        for (int i = 0; i < y.size(); i++)
        {
            if (i >= x.size())
                break;
            fout << x[i] << " " << y[i] << "\n";
        }
        fout.close();

        // Determine the range of the data
        fprintf(gnuplotPipe, "stats '%s' using 1:2 nooutput\n", filename.c_str());
        fprintf(gnuplotPipe, "y_offset = (STATS_max_y - STATS_min_y) * 0.05\n");
        fprintf(gnuplotPipe, "set yrange [STATS_min_y - y_offset:STATS_max_y + y_offset]\n");

        fprintf(gnuplotPipe, "set boxwidth %f relative\n", bin_width);
        fprintf(gnuplotPipe, "set style fill solid %f\n", opacity);

        if (std::string(color) == "auto")
            fprintf(gnuplotPipe, "plot '%s' using 2:xtic(1) title '%s'\n", filename.c_str(), title);
        else
            fprintf(gnuplotPipe, "plot '%s' using 2:xtic(1) linecolor '%s' title '%s'\n", filename.c_str(), color, title);

        cnt_files++;
    }

    template <typename T2>
    inline void createPlot(const std::vector<T2> &y, const char *line_title = "", const char *linecolor = "auto", const bool set_range = true)
    {
        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);
        for (int i = 0; i < y.size(); i++)
            fout << i << " " << y[i] << "\n";
        fout.close();

        if (set_range)
        {
            fprintf(gnuplotPipe, "stats '%s' using 1:2 nooutput\n", filename.c_str());
            fprintf(gnuplotPipe, "x_offset = (STATS_max_x - STATS_min_x) * 0.05\n");
            fprintf(gnuplotPipe, "y_offset = (STATS_max_y - STATS_min_y) * 0.05\n");
            fprintf(gnuplotPipe, "set xrange [STATS_min_x - x_offset:STATS_max_x + x_offset]\n");
            fprintf(gnuplotPipe, "set yrange [STATS_min_y - y_offset:STATS_max_y + y_offset]\n");
        }
        fprintf(gnuplotPipe, "plot ");
        if (linecolor == "auto")
            fprintf(gnuplotPipe, "\"%s\" using 1:2 smooth unique with lines title '%s'", filename.c_str(), line_title);
        else
            fprintf(gnuplotPipe, "\"%s\" using 1:2 smooth unique with lines linecolor '%s' title '%s'", filename.c_str(), linecolor, line_title);

        cnt_files++;
    }

    template <typename T1, typename T2>
    inline void createPlot(const std::vector<T1> &x, const std::vector<T2> &y, const char *line_title = "", const char *linecolor = "auto", const T1 shift = static_cast<T1>(0), const bool set_range = true)
    {
        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);
        for (int i = 0; i < x.size(); i++)
        {
            if (i >= y.size())
                break;
            fout << x[i] + shift << " " << y[i] << "\n";
        }
        fout.close();

        if (set_range)
        {
            fprintf(gnuplotPipe, "stats '%s' using 1:2 nooutput\n", filename.c_str());
            fprintf(gnuplotPipe, "x_offset = (STATS_max_x - STATS_min_x) * 0.05\n");
            fprintf(gnuplotPipe, "y_offset = (STATS_max_y - STATS_min_y) * 0.05\n");
            fprintf(gnuplotPipe, "set xrange [STATS_min_x - x_offset:STATS_max_x + x_offset]\n");
            fprintf(gnuplotPipe, "set yrange [STATS_min_y - y_offset:STATS_max_y + y_offset]\n");
        }
        fprintf(gnuplotPipe, "plot ");
        if (linecolor == "auto")
            fprintf(gnuplotPipe, "\"%s\" using 1:2 smooth unique with lines title '%s'", filename.c_str(), line_title);
        else
            fprintf(gnuplotPipe, "\"%s\" using 1:2 smooth unique with lines linecolor '%s' title '%s'", filename.c_str(), linecolor, line_title);

        cnt_files++;
    }

    template <typename T2>
    inline void addPlot(const std::vector<T2> &y, const char *line_title, const char *linecolor = "auto")
    {
        std::string filename = std::to_string(cnt_files) + ".dat";
        std::ofstream fout(filename);
        for (int i = 0; i < y.size(); i++)
            fout << i << " " << y[i] << "\n";
        fout.close();

        if (linecolor == "auto")
            fprintf(gnuplotPipe, ", \"%s\" using 1:2 smooth unique with lines title '%s'", filename.c_str(), line_title);
        else
            fprintf(gnuplotPipe, ", \"%s\" using 1:2 smooth unique with lines linecolor '%s' title '%s'", filename.c_str(), linecolor, line_title);

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
        fout.close();

        if (linecolor == "auto")
            fprintf(gnuplotPipe, ", \"%s\" using 1:2 smooth unique with lines title '%s'", filename.c_str(), line_title);
        else
            fprintf(gnuplotPipe, ", \"%s\" using 1:2 smooth unique with lines linecolor '%s' title '%s'", filename.c_str(), linecolor, line_title);

        cnt_files++;
    }

private:
    FILE *gnuplotPipe;
    int cnt_files = 0;
};
