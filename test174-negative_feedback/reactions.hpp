#pragma once

#include "gsp.hpp"


namespace gsp
{
    class simple_association : public gsp::reaction
    {
    public:
        struct param
        {
            size_t reactant1;
            size_t reactant2;
            size_t product;
            double rate;
        };

        simple_association(param const& ps)
            : _reactant1{ps.reactant1}
            , _reactant2{ps.reactant2}
            , _product{ps.product}
            , _rate{ps.rate}
        {
        }

        std::vector<size_t> dependency() const override
        {
            return {_reactant1, _reactant2};
        }

        std::vector<gsp::change> changeset(gsp::state const&) const override
        {
            return {
                gsp::change {_reactant1, -1},
                gsp::change {_reactant2, -1},
                gsp::change {_product, 1},
            };
        }

        double rate(gsp::state const& state) const override
        {
            return _rate * double(state.species[_reactant1] * state.species[_reactant2]);
        }

    private:
        size_t _reactant1;
        size_t _reactant2;
        size_t _product;
        double _rate;
    };

    class simple_dissociation : public gsp::reaction
    {
    public:
        struct param
        {
            size_t reactant;
            size_t product1;
            size_t product2;
            double rate;
        };

        simple_dissociation(param const& ps)
            : _reactant{ps.reactant}
            , _product1{ps.product1}
            , _product2{ps.product2}
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
                gsp::change {_product1, 1},
                gsp::change {_product2, 1},
            };
        }

        double rate(gsp::state const& state) const override
        {
            return _rate * double(state.species[_reactant]);
        }

    private:
        size_t _reactant;
        size_t _product1;
        size_t _product2;
        double _rate;
    };

    class linear_mediated_generation : public gsp::reaction
    {
    public:
        struct param
        {
            size_t species;
            double rate;
            size_t mediator;
        };

        linear_mediated_generation(param const& ps)
            : _species{ps.species}
            , _rate{ps.rate}
            , _mediator{ps.mediator}
        {
        }

        std::vector<size_t> dependency() const override
        {
            return {_mediator};
        }

        std::vector<gsp::change> changeset(gsp::state const&) const override
        {
            return {gsp::change {_species, 1}};
        }

        double rate(gsp::state const& state) const override
        {
            return _rate * double(state.species[_mediator]);
        }

    private:
        size_t _species;
        double _rate;
        size_t _mediator;
    };
};
