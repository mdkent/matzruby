require 'rdoc/generators/html_generator'

##
# Generate XML output as one big file

class RDoc::Generators::XMLGenerator < RDoc::Generators::HTMLGenerator

  ##
  # Standard generator factory

  def self.for(options)
    new(options)
  end

  def initialize(*args)
    super
  end

  ##
  # Build the initial indices and output objects
  # based on an array of TopLevel objects containing
  # the extracted information.

  def generate(info)
    @info       = info
    @files      = []
    @classes    = []
    @hyperlinks = {}

    build_indices
    generate_xml
  end

  ##
  # Generate:
  #
  # * a list of HtmlFile objects for each TopLevel object.
  # * a list of HtmlClass objects for each first level
  #   class or module in the TopLevel objects
  # * a complete list of all hyperlinkable terms (file,
  #   class, module, and method names)

  def build_indices
    @info.each do |toplevel|
      @files << RDoc::Generators::HtmlFile.new(toplevel, @options, RDoc::Generators::FILE_DIR)
    end

    RDoc::TopLevel.all_classes_and_modules.each do |cls|
      build_class_list(cls, @files[0], RDoc::Generators::CLASS_DIR)
    end
  end

  def build_class_list(from, html_file, class_dir)
    @classes << RDoc::Generators::HtmlClass.new(from, html_file, class_dir, @options)
    from.each_classmodule do |mod|
      build_class_list(mod, html_file, class_dir)
    end
  end

  ##
  # Generate all the HTML. For the one-file case, we generate
  # all the information in to one big hash

  def generate_xml
    values = {
      'charset' => @options.charset,
      'files'   => gen_into(@files),
      'classes' => gen_into(@classes)
    }

    # this method is defined in the template file
    write_extra_pages if defined? write_extra_pages

    template = RDoc::TemplatePage.new(RDoc::Page::ONE_PAGE)

    if @options.op_name
      opfile = File.open(@options.op_name, "w")
    else
      opfile = $stdout
    end
    template.write_html_on(opfile, values)
  end

  def gen_into(list)
    res = []
    list.each do |item|
      res << item.value_hash
    end
    res
  end

  def gen_file_index
    gen_an_index(@files, 'Files')
  end

  def gen_class_index
    gen_an_index(@classes, 'Classes')
  end

  def gen_method_index
    gen_an_index(RDoc::Generators::HtmlMethod.all_methods, 'Methods')
  end

  def gen_an_index(collection, title)
    res = []
    collection.sort.each do |f|
      if f.document_self
        res << { "href" => f.path, "name" => f.index_name }
      end
    end

    return {
      "entries" => res,
      'list_title' => title,
      'index_url'  => main_url,
    }
  end

end

