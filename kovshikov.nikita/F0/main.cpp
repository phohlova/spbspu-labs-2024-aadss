#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include "createGraph.hpp"
#include "outMessage.hpp"

int main()
{
  using namespace kovshikov;
  Tree< std::string, Graph> graphsList;
  Tree< std::string, std::function< void(Tree< std::string, Graph >&, std::istream&) > > interaction;

  {
    using namespace std::placeholders;
    interaction["create"] = std::bind(createGraph, _1, _2);
    interaction["lonely"] = std::bind(createLonely, _1, _2);
    interaction["delete"] = std::bind(deleteGraph, _1, _2);
    interaction["work"] = std::bind(workWith, _1, _2);
  }

  Tree< std::string, std::function< void(const Tree< std::string, Graph >&, std::ostream&) > > outGraph;

  {
    using namespace std::placeholders;
    outGraph["list"] = std::bind(outputGraphs, _1, _2);
    outGraph["output"] = std::bind(outputVertexes, _1, _2);
  }

  std::string command;

  while(std::cin >> command)
  {
    if(command == "help")
    {
      outDescription(std::cout);
    }
    else
    {
      try
      {
        interaction.at(command)(graphsList, std::cin);
      }
      catch(const std::out_of_range& error)
      {
        try
        {
          outGraph.at(command)(graphsList, std::cout);
        }
        catch(const std::out_of_range& error)
        {
          outInvalid(std::cout);
          std::cin.clear();
          std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
      }
      catch(const std::logic_error& error)
      {
        std::cout << error.what() << "\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
}
