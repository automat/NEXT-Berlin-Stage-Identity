#include "data/quote/json/QuoteParser.h"

namespace next{

    QuoteJson QuoteParser::ReadQuote(const JsonTree& tree){
        QuoteJson quote;
        quote.str               = tree.getChild("string").getValue<string>();
        quote.format.scale      = tree.getChild("format.scale").getValue<float>();
        quote.format.padding[0] = tree.getChild("format.padding")[0].getValue<int>();
        quote.format.padding[1] = tree.getChild("format.padding")[1].getValue<int>();
        quote.format.padding[2] = tree.getChild("format.padding")[2].getValue<int>();
        quote.format.padding[3] = tree.getChild("format.padding")[3].getValue<int>();
        string alignStr         = tree.getChild("format.align").getValue<string>();
        quote.format.align      = alignStr == "center" ? QuoteAlign::CENTER :
                alignStr == "left"   ? QuoteAlign::LEFT :
                        QuoteAlign::RIGHT;
        quote.format.balance    = tree.getChild("format.balance").getValue<bool>();
        return quote;
    }

    bool QuoteParser::IsValidQuoteObj(const JsonTree& tree, string* msg){
        if (!tree.hasChildren() ) {
            *msg = "Empty object.";
            return false;
        }

        if(!tree.hasChild("format")){
            *msg = "Quote has no format.";
            return false;
        }

        if(!tree.hasChild("string")){
            *msg = "Quote has no string.";
            return false;
        }

        const JsonTree& format = tree.getChild("format");
        if(!format.hasChild("align")){
            *msg = "Quote format align not set.";
            return false;
        }

        if(!format.hasChild("balance")){
            *msg = "Quote format balance not set.";
            return false;
        }

        if(!format.hasChild("padding")){
            *msg = "Quote format padding not set.";
            return false;
        }

        if(!format.hasChild("scale")){
            *msg = "Quote format scale not set.";
            return false;
        }

        try {
            format.getChild("align").getValue<string>();
        } catch (JsonTree::ExcNonConvertible& exc) {
            *msg = "Quote format align is not of type float.";
            return false;
        }

        string align = format.getChild("align").getValue<string>();
        if(align != "center" && align != "left" && align != "right"){
            *msg = "Quote format align has wrong property.";
            return false;
        }

        try {
            format.getChild("balance").getValue<bool>();
        } catch (JsonTree::ExcNonConvertible& exc){
            *msg = "Quote format align is not of type bool.";
            return false;
        }

        if(format.getChild("padding").getChildren().size() != 4){
            *msg = "Quote format padding is not of type float.";
            return false;
        }

        for(int i = 0; i < 4; ++i){
            try{
                format.getChild("padding")[i].getValue<int>();
            } catch (JsonTree::ExcNonConvertible& exc){
                *msg = "Quote format padding " + toString(i) + " is not of type int.";
                return false;
            }
        }
        return true;
    }

    bool QuoteParser::LoadJson(const string& filepath, vector<QuoteJson>* data, string* msg){
        static bool _init = false;
        static JsonTree::ParseOptions _parseOptions;
        if(!_init){
            _parseOptions.ignoreErrors(false);
            _init = true;
        }

        bool valid = true;
        JsonTree temp;

        try{ // check if json is valid
            temp = JsonTree(loadFile(filepath),_parseOptions);
        } catch ( JsonTree::ExcJsonParserError& exc){
            *msg  = exc.what();
            valid = false;
        }

        // check if quotes are valid
        string errMsg;
        if(valid){
            if(!temp.hasChild("quotes")){
                *msg = "Error! No quote array set.";
                valid  = false;
            } else {
                const JsonTree& _quotes = temp.getChild("quotes");
                for(const auto& quote : _quotes){
                    if(!IsValidQuoteObj(quote,&errMsg)){
                        *msg = errMsg;
                        valid  = false;
                        break;
                    }
                }
            }
        }

        if(valid){
            data->clear();
            const JsonTree& _quotes = temp.getChild("quotes");
            for(const auto& quote : _quotes){
                data->push_back(ReadQuote(quote));
            }
        }

        return valid;
    }
}


