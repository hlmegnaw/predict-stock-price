#include <iostream>
#include <vector>
#include <climits>
#include <limits>


using namespace std;

vector<int> prices;
int dayLimit = 5; // Default window size for analysis
vector<string> analysisHistory; // To store analysis history

// Utility function to get current timestamp
string getCurrentTimestamp() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return buf;
}

// Function to add analysis to history
void addToHistory(const string& analysis) {
    analysisHistory.push_back(getCurrentTimestamp() + " - " + analysis);
}


}

// Main menu function
int main() {
    int choice;

    do {
        cout << "\nStock Market Analyzer\n";
        cout << "1. Load Stock Prices\n";
        cout << "2. Find Best Buy & Sell Days (Profit Maximizer)\n";
       cout << "3. Predict next price\n";

        cout << "4. Smart Price Changes (Portfolio Planner)\n";
        cout << "5. Compare Day-to-Day Price Changes (Correlation Finder)\n";
        cout << "6. Detect Longest Upward Trend (Trend Tracker)\n";
        cout << "7. Price Volatility Analysis\n";
        cout << "8. Change Analysis Range (Current: " << dayLimit << " days)\n";
        cout << "9. View Analysis History\n";
        cout << "10. Exit\n";
        cout << "Enter your choice (1-9): ";

        cin >> choice;

        switch (choice) {
            case 1: inputPrices(); break;
            case 2: calculateMaxProfit(); break;
            case 3: predict_nextprice(); break;
            case 4: InvestmentStrategy(); break;
            case 5: PriceChanges(); break;
            case 6: trendAnalysisBFS(); break;
            case 7: priceVolatilityAnalysis(); break;
            case 8: changeDayLimit(); break;
            case 9: viewAnalysisHistory(); break;
            case 10:
                cout << "Saving analysis history...\n";
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter 1-9.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (choice != 9);

    return 0;
}
