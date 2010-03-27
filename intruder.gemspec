Gem::Specification.new do |s|
  s.name = %q{intruder}
  s.version = "0.2.0"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Pascal Friederich"]
  s.date = %q{2010-03-11}
  s.description = %q{Communicate with erlang from within ruby natively}
  s.email = %q{paukul@gmail.com}
  s.extra_rdoc_files = ["README.md"]
  s.files = ["README.md", "findei.erl"] + Dir["src/*"] + Dir["lib/**/*"]
  s.homepage = %q{http://github.com/paukul/intruder}
  s.require_paths = ["lib"]
  s.rubygems_version = %q{1.3.5}
  s.extensions << 'extconf.rb'
  s.summary = %q{Communicate with erlang from within ruby as a ruby c erlang node}
  s.add_development_dependency('minitest', '>=1.5.0')
  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
