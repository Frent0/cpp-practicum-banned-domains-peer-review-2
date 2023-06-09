#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>

using namespace std::literals::string_view_literals;

class Domain {
public:

    explicit Domain(const std::string& name) {
        std::string String = "." + name;
        std::reverse(String.begin(), String.end());
        name_ = std::move(String);
    }

    bool operator==(const Domain& other) const {
        return std::lexicographical_compare(name_.begin(), name_.end(),
            other.name_.begin(), other.name_.end());
    }

    bool IsSubdomain(const Domain& other) const {
        if (other.name_.size() <= name_.size()) {
            return std::equal(other.name_.begin(),
                other.name_.end(),
                name_.begin());
        }
        return false;
    }

    const std::string& GetDomain() const {
        return name_;
    }

private:
    std::string name_;
};

class DomainChecker {
public:

    template <typename Iterator>
    DomainChecker(const Iterator start, const Iterator end) {
        forbidden_.assign(start, end);

        std::sort(forbidden_.begin(), forbidden_.end(),
            [](const Domain& lhs, const Domain& rhs) { return lhs == rhs; });

        auto uniqueDomainsIt = std::unique(forbidden_.begin(), forbidden_.end(),
            [](const Domain& lhs, const Domain& rhs) { return rhs.IsSubdomain(lhs); });

        forbidden_.erase(uniqueDomainsIt, forbidden_.end());
    }

    bool IsForbidden(const Domain& domain_check) {
        if (!forbidden_.empty()) {
            auto domain_pos_it = std::upper_bound(forbidden_.begin(), forbidden_.end(),
                domain_check, [](const Domain& lhs, const Domain& rhs) {
                    return lhs.GetDomain() < rhs.GetDomain();
                });

            if (domain_pos_it == forbidden_.begin()) {
                return domain_check.IsSubdomain(forbidden_[0]);
            }
            else {
                size_t index = std::distance(forbidden_.begin(), domain_pos_it);
                return domain_check.IsSubdomain(forbidden_[index - 1]);
            }
        }
        return false;
    }

private:
    std::vector<Domain> forbidden_;
};

std::vector<Domain> ReadDomains(std::istream& input, size_t count) {
    std::vector<Domain> domains;
    std::string line;
    for (size_t i = 0; i < count; ++i) {
        getline(input, line);
        domains.emplace_back(line);
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(std::istream& input) {
    std::string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    for (const Domain& domain : test_domains) {
        std::cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << std::endl;
    }
    return 0;
}
