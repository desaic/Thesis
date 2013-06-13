#ifndef TEXT_RANGE_HPP
#define TEXT_RANGE_HPP
class TextPos {
public:
    const char* GetFilename() const { return mFilename; }
    unsigned int GetLine() const { return mLine; }
    unsigned int GetColumn() const { return mColumn; }

    void SetLine(unsigned int lineNum)
    {
        mLine = lineNum;
        mColumn = 1;
    }

    void SetFilename(const char* filename) { mFilename = filename; }
    void operator+=(unsigned int numChars) { mColumn += numChars; }

    void Newline(unsigned int numLines = 1) 
    { 
        mLine += numLines;
        mColumn = 1; 
    }

    TextPos(const char* filename, unsigned int line=1, unsigned int column=1)
        : mFilename(filename)
        , mLine(line)
        , mColumn(column)
    {
    }

    TextPos():mLine(1), mColumn(1) { }

private:
    const char* mFilename;
    unsigned int mLine, mColumn;

};

class TextRange {
public:
    const TextPos& GetStart() const { return mStart; }

    const TextPos& GetEnd() const { return mEnd; }

    void SetEnd(const TextPos& end) { mEnd = end; }

    void SetLine(unsigned int lineNum)
    {
        mStart.SetLine(lineNum);
        mEnd.SetLine(lineNum);
    }

    void SetFilename(const char* filename)
    {
        mStart.SetFilename(filename);
        mEnd.SetFilename(filename);
    }

    void operator+=(unsigned int numChars) { mEnd += numChars; }

    void Newline(unsigned int numLines = 1) { mEnd.Newline(numLines); }

    void Skip() { mStart = mEnd; }

    void Follow(const TextRange& previousLoc) 
    {
        mStart = mEnd = previousLoc.GetEnd();
    }

    void Combine(const TextRange& first, const TextRange& last)
    {
        mStart = first.GetStart();
        mEnd = last.GetEnd();
    }

    TextRange Union(const TextRange& other)
    {
        return TextRange(GetStart(), other.GetEnd());
    }

    TextRange(const TextPos& start, const TextPos& end)
        : mStart(start)
        , mEnd(end)
    {
    }

    TextRange(const TextPos& pos)
        : mStart(pos)
        , mEnd(pos)
    {
    }

    TextRange() { } 

private:
    TextPos mStart, mEnd;
};

#endif