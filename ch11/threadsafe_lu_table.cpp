#include <mutex>
#include <list>
#include <shared_mutex>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <iostream>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lu_table {
private:
    class bucket_type {
    private:
        using value_type = std::pair<Key, Value>;
        using bucket_data = std::list<value_type>;
        using iterator = typename bucket_data::iterator;

        bucket_data data_;
        mutable std::shared_mutex m_;

    private:
        // !!! this method is not const annotated. Otherwise lucky debugging...
        iterator find_entry_for(const Key& key) {
            return std::find_if(data_.begin(), data_.end(),
                    [&] (const value_type& p) { return p.first == key;  }
            );
        }

    public:
        // I need to use data_ and m_ in the main class
        friend class threadsafe_lu_table;

        Value value_for(const Key& key, const Value& default_value) const {
            std::shared_lock<std::shared_mutex> l{m_};
            auto it = find_entry_for(key);
            return (it == data_.end()) ? default_value : it->second;
        }

        void add_or_update_mapping(const Key& key, const Value& value) {
            std::unique_lock<std::shared_mutex> l{m_};
            auto it = find_entry_for(key);
            if (it == data_.end()) {
                data_.push_back(value_type{key, value});
            } else {
                it->second = value;
            }
        }

        void remove_mapping(const Key& key) {
            std::unique_lock<std::shared_mutex> l{m_};
            auto it = find_entry_for(key);
            if (it != data_.end()) {
                data_.erase(key);
            }
        }
    };

    std::vector< std::unique_ptr<bucket_type> > buckets_;
    Hash hasher_;

    bucket_type& get_bucket(const Key& key) const {
        auto index = hasher_(key) % buckets_.size();
        return *buckets_[index];
    }

public:
    using key_type = Key;
    using value_type = Value;
    using hash_type = Hash;

    threadsafe_lu_table(unsigned int n_buckets = 19, hash_type hasher = Hash())
     : buckets_(n_buckets), hasher_(hasher) {
        std::for_each(buckets_.begin(), buckets_.end(), [] (auto&& up) { up.reset(new bucket_type); });
    }

    threadsafe_lu_table(const threadsafe_lu_table& other) = delete;
    threadsafe_lu_table& operator= (const threadsafe_lu_table& other) = delete;

    value_type value_for(const Key& key, const value_type& default_value) const {
        return get_bucket(key).value_for(key, default_value);
    }

    void add_or_update_mapping(Key const& key,Value const& value) {
        get_bucket(key).add_or_update_mapping(key, value);
    }

    void remove_mapping(const Key& key) {
        get_bucket(key).remove_mapping(key);
    }

    // one more important method: view as a std::map<>

    std::map<key_type, value_type> get_map() const {
        auto nb = buckets_.size();
        std::vector<std::unique_lock<std::shared_mutex>> locks(nb);
        std::transform(buckets_.begin(), buckets_.end(), locks.begin(),
            [this] (auto&& b) { return std::unique_lock<std::shared_mutex>((*b).m_); });

        std::map<key_type, value_type> res;
        std::for_each(buckets_.begin(), buckets_.end(), [&] (auto&& b) {
            std::transform((*b).data_.begin(), (*b).data_.end(),
                std::inserter(res, res.begin()), [] (auto&& p) { return p; });
        });
        return res;
    }
};

int main() {
    threadsafe_lu_table<int, std::string> lut;

    auto value_1 = std::string("lol");
    auto value_2 = std::string("kek");

    lut.add_or_update_mapping(3, value_1);
    lut.add_or_update_mapping(5, value_2);

    auto m = lut.get_map();

    for (auto&& p : m) {
        std::cout << p.first << " : " << p.second << std::endl; 
    }
}
