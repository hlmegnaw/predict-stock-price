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


void inputPrices() {
    int n;
    cout << "Enter number of days of stock data: ";
    cin >> n;
    if (n < 2) {
        cout << "You need at least 2 days of data.\n";
        return;
    }
    prices.clear(); // Reset previous data
    prices.resize(n);
    cout << "Enter " << n << " stock prices (space-separated):\n";
    for (int i = 0; i < n; ++i) {
      cout << "Enter price "<<i+1<<": ";
        cin >> prices[i];
        cout<<endl;
        while (prices[i] < 0) {
            cout << "Price cannot be negative. Please enter a valid price: ";
            cin >> prices[i];
        }
    }
    int back;
    cout << "Enter 0 to go back to menu: ";
    cin >> back;
    while (back != 0) {
        cout << "Invalid input. Please enter 0 to return: ";
        cin >> back;
    }
    cout << "Returning to menu...\n";
    cout << "Stock data loaded successfully.\n";
    addToHistory("Loaded " + to_string(n) + " days of stock data");
}
void calculateMaxProfit() {
    if (prices.empty()) {
        cout << "No stock data entered. Please enter data first.\n";
        int choice;
        cout << "Do you want to enter data now? (enter 1 to Yes or enter 0 to No): \n";
        cin >> choice;
        
        if (choice == 1) {
            inputPrices();
        }
        else {
            cout << "Returning to main menu...\n";
        }
        return;
    }

    int minPrice = INT_MAX;
    int maxProfit = 0;
    int buyDay = 0, sellDay = 0;
    int currentMinDay = 0;

    cout << "\nAnalyzing stock prices for maximum profit...\n";
    for (int i = 0; i < prices.size(); ++i) {
        for (int j = i + 1; j < prices.size(); ++j) {
            int profit = prices[j] - prices[i];
            if (profit > maxProfit) {
                maxProfit = profit;
                buyDay = i;
                sellDay = j;
            }
        }
    }

    if (maxProfit > 0) {
        cout << "Max Profit = " << maxProfit << "\n";
        cout << "Buy on Day " << buyDay + 1 << " (Price = " << prices[buyDay] << ")\n";
        cout << "Sell on Day " << sellDay + 1 << " (Price = " << prices[sellDay] << ")\n";
        addToHistory("Max Profit Analysis: Profit=" + to_string(maxProfit) + 
                     ", Buy Day=" + to_string(buyDay+1) + 
                     ", Sell Day=" + to_string(sellDay+1));
    } else {
        cout << "No profitable opportunity found.\n";
        addToHistory("Max Profit Analysis: No profitable opportunity found");
    }
}

// Function for investment strategy
void InvestmentStrategy() {
    if (prices.size() < 3) {
        cout << "Need at least 3 days for portfolio optimization.\n";
        return;
    }

    vector<vector<int>> graph = {
        {0, 1, INT_MAX},
        {INT_MAX, 0, 1},
        {INT_MAX, INT_MAX, 0}
    };

    for (int i = 0; i < prices.size() - 1; i++) {
        int profit = prices[i+1] - prices[i];
        graph[0][1] = min(graph[0][1], -profit);
        if (i < prices.size() - 2) {
            graph[1][2] = min(graph[1][2], -profit);
        }
    }

    vector<int> cost(3, INT_MAX);
    vector<int> path(3);
    cost[0] = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (graph[i][j] != INT_MAX && cost[i] + graph[i][j] < cost[j]) {
                cost[j] = cost[i] + graph[i][j];
                path[j] = i;
            }
        }
    }

    cout << "Optimal portfolio path cost: " << -cost[2] << " (profit)\n";
    cout << "Path: Buy -> Hold -> Sell\n";
    addToHistory("Portfolio Optimization: Profit=" + to_string(-cost[2]));
}

// Function for price changes analysis
void PriceChanges() {
    if (prices.empty()) {
        cout << "No data for correlation analysis.\n";
        return;
    }

    int n = prices.size();
    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) dist[i][j] = 0;
            else if (abs(i-j) <= dayLimit) {
                dist[i][j] = abs(prices[j] - prices[i]);
            }
        }
    }
 for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    int minDist = INT_MAX;
    pair<int,int> bestPair;
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if (dist[i][j] < minDist) {
                minDist = dist[i][j];
                bestPair = {i+1, j+1};
            }
        }
    }

    cout << "Most correlated days: " << bestPair.first
         << " and " << bestPair.second << " (Δ: " << minDist << ")\n";
    addToHistory("Correlation Analysis: Days " + to_string(bestPair.first) +
                 " & " + to_string(bestPair.second) + " (Δ=" + to_string(minDist) + ")");
}

void changeDayLimit() {
    cout << "Current day limit: " << dayLimit << "\n";
    cout << "Enter new day limit (2-30): ";
    int newLimit;
    cin >> newLimit;
    
    while (newLimit < 2 || newLimit > 30) {
        cout << "Invalid range. Please enter between 2 and 30: ";
        cin >> newLimit;
    }
    
    dayLimit = newLimit;
    cout << "Day limit set to " << dayLimit << " days\n";
    addToHistory("Changed day limit to " + to_string(dayLimit));
}

// New function for Menu 6: Price Volatility Analysis
void priceVolatilityAnalysis() {
    if (prices.empty()) {
        cout << "No data for volatility analysis.\n";
        return;
    }

    double sum = 0.0;
    for (int price : prices) {
        sum += price;
    }
    double mean = sum / prices.size();

    double variance = 0.0;
    for (int price : prices) {
        variance += pow(price - mean, 2);
    }
    variance /= prices.size();
    double stdDev = sqrt(variance);

    cout << fixed << setprecision(2);
    cout << "Price Volatility Analysis:\n";
    cout << "Mean price: " << mean << "\n";
    cout << "Standard deviation: " << stdDev << "\n";
    cout << "Volatility index: " << (stdDev / mean) * 100 << "%\n";
    
    if ((stdDev / mean) * 100 > 15.0) {
        cout << "Warning: High volatility detected!\n";
    } else {
        cout << "Market shows stable trends\n";
    }
    
    addToHistory("Volatility Analysis: SD=" + to_string(stdDev) + 
                 ", Volatility=" + to_string((stdDev / mean) * 100) + "%");
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
