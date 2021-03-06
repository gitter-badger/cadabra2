
#pragma once

#include "Props.hh"

class ImplicitIndex : virtual public property {
	public:
		virtual bool parse(const Kernel&, keyval_t&) override;
		virtual std::string name() const override;
		virtual std::string unnamed_argument() const override { return "name"; };
		virtual void latex(std::ostream& str) const override;

		std::vector<std::string> set_names;
};

