#include "BFS.h"
#include "Plotting.h"
#include "Env.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <set>
#include <unordered_set>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include <math.h>


BFS::PointSetPair BFS::searching()
{
    BFS::PointSetPair pathVisitedPair;
    _parent[_xI] = _xI;
    _g[_xI] = 0.0;
    _g[_xG] = std::numeric_limits<double>::infinity();
    
    std::cout << "Starting at:" << std::endl;
    std::cout << "xI: " << _xI.first << ", " << _xI.second << std::endl;

    _open.push(std::make_pair(0, _xI));

    int count = 0;

    while(!_open.empty())
    {
        BFS::Point s = _open.top().second;
        _xC = s;
        _open.pop();
        _closed.insert(s);
        
        
        if (count % 40 == 0)
        {
            // Plot visited points and path.
            this->displayPlots();
            cv::waitKey(10); // Pause for a short time
        }

        count++;

        if(s == _xG)
        {           
            std::cout << "Goal found!" << std::endl; 
            break;
        }

        BFS::PointVector neighbours = this->getNeighbours(s);
        for(auto s_next : neighbours)
        {
            double new_cost = _g[s] + this->cost(s, s_next);

            if(_closed.find(s_next) == _closed.end())
            {
                _g[s_next] = std::numeric_limits<double>::infinity();
            }



            if(new_cost < _g[s_next])
            {
                _g[s_next] = new_cost;
                _parent[s_next] = s;
                double priority = 0;
                if (_open.empty())
                {
                    priority = 0;
                }
                else
                {
                    priority = _open.top().first + 1;
                }
                _open.push(std::make_pair(priority, s_next));
            }
        }
    }

    // Plot visited points and path.
    _plot.plot_visited(_closed);
    _plot.plot_path(this->extractPath(_parent));
    _plot.show_image();
    cv::waitKey(0);
    
    pathVisitedPair.first = this->extractPath(_parent);
    pathVisitedPair.second = _closed;
    
    return pathVisitedPair;
}