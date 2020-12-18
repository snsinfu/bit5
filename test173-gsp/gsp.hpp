#pragma once

#include <cstddef>
#include <memory>
#include <ostream>
#include <random>
#include <vector>


namespace gsp
{
    using std::size_t;


    struct state
    {
        std::vector<int> species;
        double           time = 0;
    };


    template<typename Char, typename Tr>
    inline std::basic_ostream<Char, Tr>&
    operator<<(std::basic_ostream<Char, Tr>& os, gsp::state const& state)
    {
        using sentry_type = typename std::basic_ostream<Char, Tr>::sentry;
        if (sentry_type sentry{os}) {
            os << state.time;
            for (auto const species : state.species) {
                os << os.widen(' ') << species;
            }
        }
        return os;
    }


    struct change
    {
        size_t species;
        int    amount;
    };


    class reaction
    {
    public:
        virtual ~reaction() = default;
        virtual std::vector<size_t>      dependency() const = 0;
        virtual std::vector<gsp::change> changeset(gsp::state const& state) const = 0;
        virtual double                   rate(gsp::state const& state) const = 0;
    };


    class simulation
    {
    public:
        explicit simulation(gsp::state const& init_state)
            : _state{init_state}
        {
            auto const species_count = init_state.species.size();
            _dependants.resize(species_count);
        }

        gsp::state const& state() const
        {
            return _state;
        }

        template<typename Reaction>
        size_t add_reaction(Reaction const& reaction)
        {
            size_t const reaction_id = _reactions.size();
            _reactions.push_back(std::make_unique<Reaction>(reaction));

            _rates.push_back(reaction.rate(_state));
            _rate_sum += _rates.back();

            for (auto const species : reaction.dependency()) {
                _dependants[species].push_back(reaction_id);
            }

            return reaction_id;
        }

        template<typename RNG>
        size_t step(RNG& random)
        {
            std::uniform_real_distribution<double> uniform;

            double const rate_threshold = uniform(random) * _rate_sum;
            double rate_cumsum = 0;
            size_t reaction_id = _reactions.size() - 1;

            for (size_t i = 0; i < _reactions.size(); i++) {
                rate_cumsum += _rates[i];
                if (rate_cumsum >= rate_threshold) {
                    reaction_id = i;
                    break;
                }
            }

            auto const& reaction = *_reactions[reaction_id];
            auto const rate = _rates[reaction_id];
            std::vector<size_t> invalidation;

            for (auto const& change : reaction.changeset(_state)) {
                _state.species[change.species] += change.amount;

                invalidation.insert(
                    invalidation.end(),
                    _dependants[change.species].begin(),
                    _dependants[change.species].end()
                );
            }

            std::sort(invalidation.begin(), invalidation.end());
            invalidation.erase(
                std::unique(invalidation.begin(), invalidation.end()),
                invalidation.end()
            );

            for (auto const i : invalidation) {
                _rate_sum -= _rates[i];
                _rates[i] = _reactions[i]->rate(_state);
                _rate_sum += _rates[i];
            }

            std::exponential_distribution<double> exponential{rate};
            _state.time += exponential(random);

            return reaction_id;
        }

    private:
        gsp::state                                  _state;
        std::vector<std::vector<size_t>>            _dependants;
        std::vector<std::unique_ptr<gsp::reaction>> _reactions;
        std::vector<double>                         _rates;
        double                                      _rate_sum = 0;
    };


    // REACTIONS -------------------------------------------------------------

    class simple_generation : public gsp::reaction
    {
    public:
        struct params
        {
            size_t species;
            double rate;
        };

        explicit simple_generation(params const& ps)
            : _species{ps.species}
            , _rate{ps.rate}
        {
        }

        std::vector<size_t> dependency() const override
        {
            return {_species};
        }

        std::vector<gsp::change> changeset(gsp::state const&) const override
        {
            return {gsp::change {_species, 1}};
        }

        double rate(gsp::state const&) const override
        {
            return _rate;
        }

    private:
        size_t _species;
        double _rate;
    };


    class simple_decay : public gsp::reaction
    {
    public:
        struct params
        {
            size_t species;
            double rate;
        };

        explicit simple_decay(params const& ps)
            : _species{ps.species}
            , _rate{ps.rate}
        {
        }

        std::vector<size_t> dependency() const override
        {
            return {_species};
        }

        std::vector<gsp::change> changeset(gsp::state const&) const override
        {
            return {gsp::change {_species, -1}};
        }

        double rate(gsp::state const& state) const override
        {
            return double(state.species[_species]) * _rate;
        }

    private:
        size_t _species;
        double _rate;
    };


    class simple_transformation : public gsp::reaction
    {
    public:
        struct params
        {
            size_t reactant;
            size_t product;
            double rate;
        };

        explicit simple_transformation(params const& ps)
            : _reactant{ps.reactant}
            , _product{ps.product}
            , _rate{ps.rate}
        {
        }

        std::vector<size_t> dependency() const override
        {
            return {_reactant};
        }

        std::vector<gsp::change> changeset(gsp::state const&) const override
        {
            return {
                gsp::change {_reactant, -1},
                gsp::change {_product, 1},
            };
        }

        double rate(gsp::state const& state) const override
        {
            return double(state.species[_reactant]) * _rate;
        }

    private:
        size_t _reactant;
        size_t _product;
        double _rate;
    };
}
