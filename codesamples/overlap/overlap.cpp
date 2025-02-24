     
       /************************************************************
        * @file   overlap.cpp
        * @author Evan Gray
        * 
        * @brief: Calculates the number of overlaps in a set of 
        *         rectangles
       *************************************************************/
       #include "overlap.h"
       
       #include &ltvector>        //std::vector
       #include &ltfstream>       //std::ifstream
       #include &ltunordered_map> //std::unordered_map
       
       int MOD = 7001; //For modulo counting
       
       /************************************************************
        * @brief: Holds the data for a rectangle
       *************************************************************/
       struct Rect
       {
         unsigned x1,y1; //Top left corner
         unsigned x2,y2; //Bottom right corner
       };
       
       /************************************************************
        * @brief: Holds the data for an event of a rect starting or 
        *         ending
       *************************************************************/
       struct Event 
       {
         unsigned y;                           //Y value of the event
         unsigned x1, x2;                      //X range of the event
         enum Type {start = 1, end = -1} type; //Type of event
       
         //Less than for sorting
         bool operator&lt(Event const& other) const {return y &lt other.y;}
       };
       
       /************************************************************
        * @brief: Holds the state of the sweep line
        *         Handles current overlap counts at each x
       *************************************************************/
       struct LineData
       {
         std::vector&ltunsigned> mCurrentFieldOverlaps;           //Holds the current overlaps at each x
         std::unordered_map&ltunsigned, unsigned> mOverlapCounts; //Holds the number of each overlap count 
                                                                //(ex: 10 overlaps of 0, 5 overlaps of 1, etc)
       
         /************************************************************
          * @brief: Construct a new Line Data object
          * 
          * @param fieldWidth: Width of the field in use
         *************************************************************/
         LineData(int fieldWidth) 
           : mCurrentFieldOverlaps(fieldWidth, 0) //Defualt all x's to 0 overlaps
           , mOverlapCounts()
         {
           //Defualt entry for fieldWidth amount of 0's
           mOverlapCounts.insert(std::pair&ltunsigned, unsigned>(0, fieldWidth));
         }
       };
       
       /************************************************************
        * @brief: Solves the overlap problem using a sweep line 
        *         algorithm
       *************************************************************/
       struct SweepLine
       {
         std::map&ltint,int> mOverlaps;            //Holds the number of overlaps of each count
         std::multimap&ltunsigned, Event> mEvents; //Holds all events in order of y
         LineData mLineData;                     //Holds the current overlap counts at each x
         unsigned mFieldWidth;                   //Width of the field in use
       
         /************************************************************
          * @brief: Construct a new Sweep Line object from a file
          * 
          * @param filename: File to read rects from
         *************************************************************/
         SweepLine(char const* filename) 
           : mOverlaps()
           , mEvents()
           , mLineData(0)
           , mFieldWidth()
         {
           //Open file
           std::ifstream in(filename);
           if (in.fail()) throw "Cannot open input file";
       
           //Read in header info
           unsigned rectCount;
           in >> mFieldWidth >> rectCount;
       
           //Read in rectangles and create events for them
           for (unsigned r = 0; r &lt rectCount; ++r)
           {
             unsigned x1, y1, x2, y2;
             in >> x1 >> y1 >> x2 >> y2;
       
             mEvents.insert(std::pair&ltint, Event>(y1, Event{y1, x1, x2, Event::start}));
             mEvents.insert(std::pair&ltint, Event>(y2 + 1, Event{y2 + 1, x1, x2, Event::end}));
           }
       
           //Set up lineData
           mLineData = LineData(mFieldWidth);
         }
       
         /************************************************************
          * @brief: Processes all events in the event list while 
          *         adjusting the overlap counts
         *************************************************************/
         void Process() 
         {
           //Go through all events in order of y
           unsigned lastY = mEvents.begin()->second.y;
           for (const auto& eventpair : mEvents) 
           {
             const Event currentEvent = eventpair.second;  
       
             //If the y value has changed
             if(currentEvent.y != lastY)
             {
               //Add current overlap counts to the totals for all y's between events and last event
               for(auto const& overlapCount : mLineData.mOverlapCounts)
               {
                 mOverlaps[overlapCount.first] += overlapCount.second * (currentEvent.y - lastY);
                 mOverlaps[overlapCount.first] %= MOD;
               }
             }
             lastY = currentEvent.y;
       
             //For all x's in the event
             for(unsigned j = currentEvent.x1; j &lt= currentEvent.x2; ++j)
             {
               //Decrement the overlap count for the value of this x
               mLineData.mOverlapCounts[mLineData.mCurrentFieldOverlaps[j]]--;
       
               //Remove entry if no more overlaps of that count
               if(mLineData.mOverlapCounts[mLineData.mCurrentFieldOverlaps[j]] == 0)
               {
                 mLineData.mOverlapCounts.erase(mLineData.mCurrentFieldOverlaps[j]);
               }
       
               //Increment the overlap count for the new value of this x
               mLineData.mOverlapCounts[mLineData.mCurrentFieldOverlaps[j] += currentEvent.type]++;
             }
           }
         }
       };
       
       /************************************************************
        * @brief: Solves the overlap problem using a sweep line 
        *         algorithm
        * 
        * @param filename: File to read rects from
        * @return std::map&ltint,int>: Map of overlap counts
       *************************************************************/
       std::map&ltint,int> overlap( char const * filename ) 
       {
         SweepLine sl(filename);
         sl.Process();
         return sl.mOverlaps;
       }